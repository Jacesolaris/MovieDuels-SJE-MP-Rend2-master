/*
===========================================================================
Copyright (C) 2013 - 2015,MovieDuels contributors

This file is part of the MovieDuels source code.

MovieDuels is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

// g_weather.c -- weather damage to players and NPCs standing outside
//
// - acid rain, as MD SP (g_active.cpp P_WorldEffects + g_combat.cpp's MOD_SLIME branch): 1 damage (MOD_SLIME) every
//   200 + 10000 * random * fizz chance ms (0.2 - 1.2 s in rain), acid_fizz on the body, NPCs that know Protect raise it
//   instead. Off with g_acidRainDamage 0.
// - lava weather (new, SP has none): 1 - 2 damage (MOD_LAVA) every 1 - 3 s. Off with g_lavaWeatherDamage 0.
//
// The weather is the server's own: the "*..." CS_EFFECTS strings it sends to the clients, tracked as they are created
// (that is what the clients' renderers get, in the same order), so it doesn't depend on any renderer or on r_weather,
// and it works on a dedicated server.

#include "g_local.h"

extern qboolean WP_ForcePowerUsable(const gentity_t* self, forcePowers_t forcePower);
extern void WP_ForcePowerStart(const gentity_t* self, forcePowers_t forcePower, int overrideAmt);

// "outside" is re-checked this often per entity (staggered so many players don't all trace on the same frame)
#define WEATHER_OUTSIDE_CHECK_MIN	300
#define WEATHER_OUTSIDE_CHECK_RAND	200

#define WEATHER_LAVA_DELAY_MIN		1000
#define WEATHER_LAVA_DELAY_MAX		3000

typedef struct gWeatherEnt_s
{
	int nextOutsideCheck;
	qboolean outside;
	int nextAcidTime;
	int nextLavaTime;
} gWeatherEnt_t;

static bgWeatherState_t g_weatherState;
static gWeatherEnt_t g_weatherEnts[MAX_GENTITIES];
static int g_acidFizzEffect;

/*
=================
G_WeatherInit

Called from G_InitGame before the map's entities spawn. On a new map the configstrings are empty, on a map_restart
they are kept (and the spawn functions find them instead of creating them), so start from what is there: the same
strings, in the same order, a client gets when it connects (CG_RegisterGraphics stops at the first empty one too).
=================
*/
void G_WeatherInit(void)
{
	memset(g_weatherEnts, 0, sizeof g_weatherEnts);
	g_acidFizzEffect = 0;

	G_WeatherRescan();
}

/*
=================
G_WeatherRescan

The gameplay weather from the configstrings, as a client that connects now gets it: all the "*" strings in index order.
The weather command blanks old strings (G_SetWeather), so empty slots are skipped, they don't end the list.
=================
*/
void G_WeatherRescan(void)
{
	BG_WeatherReset(&g_weatherState);

	for (int i = 1; i < MAX_FX; i++)
	{
		char s[MAX_STRING_CHARS];

		trap->GetConfigstring(CS_EFFECTS + i, s, sizeof s);

		if (s[0] == '*')
		{
			BG_WeatherParseCommand(&g_weatherState, s);
		}
	}
}

/*
==============================================================================

SERVER WEATHER

The weather everybody gets is the "*<command>" strings in the CS_EFFECTS configstrings. Clients run them in index
order (live as they change, all of them when they connect: CG_ApplyWeather), and the game runs them through
bg_weather.c for the acid rain / lava weather damage (G_WeatherRescan).

The game sets the weather from the map's Weather/<mapname>.cfg file (G_WeatherLoadMapDefault, at map start) and with
the "weather" command (G_WeatherCommand_f, server console or an allowed client). The strings it creates for this are
"owned": the next change blanks them (the 64 effect slots would run out otherwise), and new ones always go after every
weather string the game doesn't own (map entities such as fx_rain), so a client that connects later runs everything in
the same order as the clients that saw the changes live.

Before this the map file was exec'd as console commands ("r_weather N"): on a listen server that only reached the host's
renderer (which turned it into r_weather's own preset, or cleared the weather with r_weather 0), on a dedicated server
it only set a cvar. Neither made a configstring, so other players and the gameplay code never had the map's weather.

==============================================================================
*/

#define G_WEATHER_MAX_OWNED		16	// configstrings one weather change may own
#define G_WEATHER_MAX_COMMANDS	8	// weather commands one change may set (the vanilla renderer has 5 clouds anyway)

static int gWeatherOwned[G_WEATHER_MAX_OWNED];
static int gWeatherNumOwned;

static qboolean G_WeatherIsOwned(const int index)
{
	for (int i = 0; i < gWeatherNumOwned; i++)
	{
		if (gWeatherOwned[i] == index)
		{
			return qtrue;
		}
	}
	return qfalse;
}

static qboolean G_WeatherStringInUse(const char* str)
{
	char s[MAX_STRING_CHARS];

	for (int i = 1; i < MAX_FX; i++)
	{
		trap->GetConfigstring(CS_EFFECTS + i, s, sizeof s);
		if (s[0] && !strcmp(s, str))
		{
			return qtrue;
		}
	}
	return qfalse;
}

// sets "*<command>" in the first free effect slot after 'after', returns the slot or -1 if there is none
static int G_WeatherAddString(const char* command, const int after)
{
	char str[MAX_QPATH];
	char s[MAX_STRING_CHARS];
	int i;

	Com_sprintf(str, sizeof str, "*%s", command);

	// keep it different from every other effect string, or G_EffectIndex would hand this slot to a map entity
	// asking for the same weather (and the next change would blank it). Trailing spaces: every weather parser skips them
	size_t len = strlen(str);
	while (G_WeatherStringInUse(str) && len < sizeof str - 1)
	{
		str[len++] = ' ';
		str[len] = '\0';
	}

	for (i = after + 1; i < MAX_FX; i++)
	{
		trap->GetConfigstring(CS_EFFECTS + i, s, sizeof s);
		if (!s[0])
		{
			break;
		}
	}

	if (i >= MAX_FX || gWeatherNumOwned >= G_WEATHER_MAX_OWNED)
	{
		trap->Print(S_COLOR_YELLOW "WARNING: no free effect configstring for the weather \"%s\"\n", command);
		return -1;
	}

	trap->SetConfigstring(CS_EFFECTS + i, str);
	gWeatherOwned[gWeatherNumOwned++] = i;
	return i;
}

// new map (not on map_restart, which keeps the configstrings): nothing is owned yet
void G_WeatherNewMap(void)
{
	gWeatherNumOwned = 0;
}

/*
=================
G_SetWeather

Replaces the weather the game set before with these commands (without the '*'). clearFirst sends a "clear" before
them, which also removes the weather of the map's entities.
=================
*/
static void G_SetWeather(const char* const* commands, const int numCommands, const qboolean clearFirst)
{
	char s[MAX_STRING_CHARS];
	int last = 0;
	int i;

	// the strings of the last change go
	for (i = 0; i < gWeatherNumOwned; i++)
	{
		trap->SetConfigstring(CS_EFFECTS + gWeatherOwned[i], "");
	}
	gWeatherNumOwned = 0;

	// the new ones go after all the other weather strings
	for (i = 1; i < MAX_FX; i++)
	{
		trap->GetConfigstring(CS_EFFECTS + i, s, sizeof s);
		if (s[0] == '*')
		{
			last = i;
		}
	}

	if (clearFirst)
	{
		last = G_WeatherAddString("clear", last);
	}

	for (i = 0; i < numCommands && last >= 0; i++)
	{
		if (commands[i] && commands[i][0])
		{
			last = G_WeatherAddString(commands[i], last);
		}
	}

	// the gameplay weather is now what a client that connects now would get
	G_WeatherRescan();
}

// a weather command for G_SetWeather from a preset number ("4") or a weather command name, NULL if it is neither
static const char* G_WeatherCommandForWord(const char* word)
{
	static const char* const names[] = {
		"lightrain", "rain", "acidrain", "heavyrain", "snow", "lava", "spacedust", "sand", "Sandstorm", "fog",
		"heavyrainfog", "light_fog", "wind", "gustingwind", "outsidepain"
	};

	if (!word || !word[0])
	{
		return NULL;
	}

	if (word[0] >= '0' && word[0] <= '9')
	{
		return BG_WeatherPresetName(atoi(word));
	}

	for (size_t i = 0; i < ARRAY_LEN(names); i++)
	{
		if (!Q_stricmp(word, names[i]))
		{
			return names[i];
		}
	}
	return NULL;
}

// the map's Weather/<mapname>.cfg ("r_weather N" lines, one weather each, as in SP): returns the number of commands
static int G_WeatherReadMapFile(const char** commands, const int maxCommands)
{
	char mapname[MAX_QPATH];
	char buf[2048];
	fileHandle_t f = 0;
	int num = 0;

	trap->Cvar_VariableStringBuffer("mapname", mapname, sizeof mapname);
	if (!mapname[0])
	{
		return 0;
	}

	const int len = trap->FS_Open(va("Weather/%s.cfg", mapname), &f, FS_READ);
	if (!f)
	{
		return 0; // most maps have none
	}
	if (len <= 0 || len >= (int)sizeof buf)
	{
		trap->FS_Close(f);
		return 0;
	}
	trap->FS_Read(buf, len, f);
	trap->FS_Close(f);
	buf[len] = '\0';

	// statements are split by line ends and ';', each is "r_weather <preset>" (or "weather <preset or name>")
	char* p = buf;
	while (*p && num < maxCommands)
	{
		char* end = p;
		char word[2][MAX_QPATH];
		int words = 0;

		while (*end && *end != '\n' && *end != '\r' && *end != ';')
		{
			end++;
		}
		const char next = *end;
		*end = '\0';

		while (*p && words < 2)
		{
			int n = 0;
			while (*p == ' ' || *p == '\t')
			{
				p++;
			}
			while (*p && *p != ' ' && *p != '\t' && n < MAX_QPATH - 1)
			{
				word[words][n++] = *p++;
			}
			word[words][n] = '\0';
			if (!n)
			{
				break;
			}
			words++;
		}

		if (words == 2 && (!Q_stricmp(word[0], "r_weather") || !Q_stricmp(word[0], "weather")))
		{
			const char* command = G_WeatherCommandForWord(word[1]);
			if (command)
			{
				commands[num++] = command;
			}
		}

		p = next ? end + 1 : end;
	}

	return num;
}

/*
=================
G_WeatherLoadMapDefault

Map start: the weather from the map's Weather/<mapname>.cfg file, for everybody, added to the weather the map's
entities set (as SP adds it). Players can show their own choice instead (r_weather) or none (cg_weather 0), see
CG_ApplyWeather; the damage always follows this server weather.
=================
*/
void G_WeatherLoadMapDefault(void)
{
	const char* commands[G_WEATHER_MAX_COMMANDS];
	char mapname[MAX_QPATH];
	static const char* const rend2NoWeatherMaps[] = {
		// as SP g_itemLoad.cpp: no file weather on these with rend2
		"yavin1", "demo", "01nar", "md2_bd_ch", "md_sn_intro_jedi", "md_ch_battledroids", "md_ep4_intro", "secbase",
		"level0", "JKG_ImperialBase", "kejim_post"
	};

	if (g_AllowWeather.integer != 1)
	{
		return;
	}

	trap->Cvar_VariableStringBuffer("mapname", mapname, sizeof mapname);
	if (com_rend2.integer == 1)
	{
		for (size_t i = 0; i < ARRAY_LEN(rend2NoWeatherMaps); i++)
		{
			if (!Q_stricmp(mapname, rend2NoWeatherMaps[i]))
			{
				return;
			}
		}
	}

	const int num = G_WeatherReadMapFile(commands, G_WEATHER_MAX_COMMANDS);
	if (num > 0)
	{
		G_SetWeather(commands, num, qfalse);
	}
}

static void G_WeatherUsage(const gentity_t* ent)
{
	const char* text =
		"usage: weather <type>   sets the server weather for everybody\n"
		"  clear (or 0, none) | default (the map's weather) | snow (1) | rain (2) | lava (3) | sand (4) |\n"
		"  heavyrainfog (5) | fog (6) | acidrain (7) | wind (8) | spacedust (9) |\n"
		"  lightrain | heavyrain | light_fog | gustingwind | Sandstorm\n";

	// the weather strings everybody runs, what the weather damage sees now and whether the asking player is outside
	char list[256] = { 0 };
	char s[MAX_STRING_CHARS];
	for (int i = 1; i < MAX_FX; i++)
	{
		trap->GetConfigstring(CS_EFFECTS + i, s, sizeof s);
		if (s[0] == '*')
		{
			Q_strcat(list, sizeof list, va(" [%s]", s + 1));
		}
	}
	const char* state = va("Server weather:%s\nacid rain %s (g_acidRainDamage %d), lava %s (g_lavaWeatherDamage %d)%s\n",
		list[0] ? list : " none", g_weatherState.acidPain ? "on" : "off", g_acidRainDamage.integer,
		g_weatherState.lava ? "on" : "off", g_lavaWeatherDamage.integer,
		ent && ent->client
		? (BG_WeatherPointOutside(ent->r.currentOrigin, ent->s.number) ? ", you are outside" : ", you are inside")
		: "");

	if (ent)
	{
		trap->SendServerCommand(ent - g_entities, va("print \"%s%s\"", text, state));
	}
	else
	{
		trap->Print("%s%s", text, state);
	}
}

/*
=================
G_WeatherCommand_f

"weather <type>" from the server console (ent NULL) or a client (g_cmds.c, which checks it is allowed).
The named types keep their old meaning (sand = wind + sand, fog = heavyrainfog), numbers are the menu's presets.
=================
*/
void G_WeatherCommand_f(const gentity_t* ent)
{
	char arg[MAX_TOKEN_CHARS];
	const char* commands[G_WEATHER_MAX_COMMANDS];
	int num = 0;

	trap->Argv(1, arg, sizeof arg);

	if (!arg[0])
	{
		G_WeatherUsage(ent);
		return;
	}

	if (!Q_stricmp(arg, "clear") || !Q_stricmp(arg, "none") || !Q_stricmp(arg, "off") || !strcmp(arg, "0"))
	{
		// nothing
	}
	else if (!Q_stricmp(arg, "default") || !Q_stricmp(arg, "map"))
	{
		// the map's weather again: its entities' weather strings (copied, they come after the "clear"), then the file
		char s[MAX_STRING_CHARS];
		static char entityWeather[G_WEATHER_MAX_COMMANDS][MAX_QPATH];

		for (int i = 1; i < MAX_FX && num < G_WEATHER_MAX_COMMANDS; i++)
		{
			trap->GetConfigstring(CS_EFFECTS + i, s, sizeof s);
			if (s[0] == '*' && !G_WeatherIsOwned(i))
			{
				Q_strncpyz(entityWeather[num], s + 1, sizeof entityWeather[num]);
				commands[num] = entityWeather[num];
				num++;
			}
		}
		if (g_AllowWeather.integer == 1)
		{
			num += G_WeatherReadMapFile(commands + num, G_WEATHER_MAX_COMMANDS - num);
		}
	}
	else if (!Q_stricmp(arg, "rain"))
	{
		commands[num++] = "rain 500";
	}
	else if (!Q_stricmp(arg, "sand") || !Q_stricmp(arg, "sandstorm"))
	{
		commands[num++] = "wind";
		commands[num++] = "sand";
	}
	else if (!Q_stricmp(arg, "fog"))
	{
		commands[num++] = "heavyrainfog";
	}
	else if (!Q_stricmp(arg, "spacedust"))
	{
		commands[num++] = "spacedust 4000";
	}
	else if (!Q_stricmp(arg, "acidrain"))
	{
		commands[num++] = "acidrain 500";
	}
	else
	{
		const char* command = G_WeatherCommandForWord(arg);
		if (!command)
		{
			G_WeatherUsage(ent);
			return;
		}
		commands[num++] = command;
	}

	G_SetWeather(commands, num, qtrue);

	if (ent)
	{
		trap->SendServerCommand(ent - g_entities, va("print \"Server weather: %s\n\"", arg));
	}
	else
	{
		trap->Print("Server weather: %s\n", arg);
	}
}

/*
=================
G_WeatherEffectCreated

A new CS_EFFECTS string starting with '*' was just set (G_FindConfigstringIndex): the clients run it as a weather
command now, so does the game.
=================
*/
void G_WeatherEffectCreated(const char* name)
{
	BG_WeatherParseCommand(&g_weatherState, name);
}

static qboolean G_WeatherEntOutside(const gentity_t* ent, gWeatherEnt_t* we)
{
	// re-check when due (or when the timer is off after a level.time jump)
	if (we->nextOutsideCheck <= level.time
		|| we->nextOutsideCheck - level.time > WEATHER_OUTSIDE_CHECK_MIN + WEATHER_OUTSIDE_CHECK_RAND)
	{
		we->outside = BG_WeatherPointOutside(ent->r.currentOrigin, ent->s.number);
		we->nextOutsideCheck = level.time + WEATHER_OUTSIDE_CHECK_MIN + Q_irand(0, WEATHER_OUTSIDE_CHECK_RAND);
	}

	return we->outside;
}

static void G_WeatherAcidFizz(const gentity_t* ent)
{
	vec3_t org;
	vec3_t dir = { 0, 0, 1 };

	// SP plays it where a G2 trace hits the body: a random spot on the upper body is close enough (and much cheaper)
	org[0] = ent->r.currentOrigin[0] + flrand(-0.5f, 0.5f) * ent->r.maxs[0];
	org[1] = ent->r.currentOrigin[1] + flrand(-0.5f, 0.5f) * ent->r.maxs[1];
	org[2] = ent->r.currentOrigin[2] + flrand(0.0f, 1.0f) * ent->r.maxs[2];

	if (!g_acidFizzEffect)
	{
		g_acidFizzEffect = G_EffectIndex("world/acid_fizz");
	}

	G_PlayEffectID(g_acidFizzEffect, org, dir);
}

/*
=================
G_WeatherDamageCheck

Players: from P_WorldEffects (ClientEndFrame). NPCs: from G_RunFrame. Does nothing (no trace) unless harmful
weather is active, its cvar is on and this entity's damage timer is due.
=================
*/
void G_WeatherDamageCheck(gentity_t* ent)
{
	if (!ent || !ent->inuse || !ent->client || ent->s.number < 0 || ent->s.number >= MAX_GENTITIES)
	{
		return;
	}

	const qboolean acid = g_weatherState.acidPain && g_acidRainDamage.integer ? qtrue : qfalse;
	const qboolean lava = g_weatherState.lava && g_lavaWeatherDamage.integer ? qtrue : qfalse;

	if (!acid && !lava)
	{
		return;
	}

	if (level.intermissiontime
		|| ent->health <= 0
		|| ent->client->noclip
		|| ent->client->tempSpectate >= level.time
		|| ent->client->sess.sessionTeam == TEAM_SPECTATOR
		|| ent->client->ps.pm_type == PM_DEAD
		|| ent->client->ps.pm_type == PM_SPECTATOR
		|| ent->client->ps.pm_type == PM_INTERMISSION
		|| ent->client->NPC_class == CLASS_VEHICLE // a parked vehicle doesn't melt
		|| ent->client->NPC_class == CLASS_HAZARD_TROOPER // as SP: "Hazard Troopers Don't Mind Acid Rain"
		|| ent->client->ps.powerups[PW_BATTLESUIT] > level.time) // as the lava / slime liquid damage
	{
		return;
	}

	gWeatherEnt_t* we = &g_weatherEnts[ent->s.number];

	// lava: the first hit comes after a random delay, not the moment the weather starts / the entity spawns
	if (lava && !we->nextLavaTime)
	{
		we->nextLavaTime = level.time + Q_irand(WEATHER_LAVA_DELAY_MIN, WEATHER_LAVA_DELAY_MAX);
	}

	// timers left over from an earlier level time (map_restart keeps nothing, but be safe)
	if (we->nextAcidTime - level.time > 15000)
	{
		we->nextAcidTime = 0;
	}
	if (we->nextLavaTime - level.time > WEATHER_LAVA_DELAY_MAX)
	{
		we->nextLavaTime = level.time + Q_irand(WEATHER_LAVA_DELAY_MIN, WEATHER_LAVA_DELAY_MAX);
	}

	// as SP: acid waits for the pain debounce too
	const qboolean acid_due = acid && we->nextAcidTime <= level.time && ent->pain_debounce_time <= level.time
		? qtrue
		: qfalse;
	const qboolean lava_due = lava && we->nextLavaTime <= level.time ? qtrue : qfalse;

	if (!acid_due && !lava_due)
	{
		return;
	}

	if (!G_WeatherEntOutside(ent, we))
	{
		return;
	}

	if (acid_due)
	{
		// as SP: 200 + 10000 * random * chance of saber fizz (0.1 in acid rain: 0.2 - 1.2 s)
		const float fizz_chance = g_weatherState.saberFizzChance > 0.0f ? g_weatherState.saberFizzChance : 0.1f;
		we->nextAcidTime = level.time + 200 + (int)(10000.0f * flrand(0.0f, 1.0f) * fizz_chance);

		if (ent->NPC && ent->client->ps.fd.forcePowersKnown & 1 << FP_PROTECT
			&& (ent->client->ps.fd.forcePowersActive & 1 << FP_PROTECT || WP_ForcePowerUsable(ent, FP_PROTECT)))
		{
			// as SP: NPCs that know Protect raise it instead of taking the damage (when they can)
			if (!(ent->client->ps.fd.forcePowersActive & 1 << FP_PROTECT))
			{
				WP_ForcePowerStart(ent, FP_PROTECT, 0);
			}
		}
		else
		{
			// half the hits fizz: it is a temp entity per hit, and the screen flash / pain already show it
			if (Q_irand(0, 1))
			{
				G_WeatherAcidFizz(ent);
			}

			G_Damage(ent, NULL, NULL, NULL, NULL, 1, 0, MOD_SLIME);
		}
	}

	if (lava_due && ent->inuse && ent->health > 0)
	{
		we->nextLavaTime = level.time + Q_irand(WEATHER_LAVA_DELAY_MIN, WEATHER_LAVA_DELAY_MAX);

		G_Damage(ent, NULL, NULL, NULL, NULL, Q_irand(1, 2), DAMAGE_NO_ARMOR | DAMAGE_NO_KNOCKBACK, MOD_LAVA);
	}
}
