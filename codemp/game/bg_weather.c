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

// bg_weather.c -- both games
//
// The SP gameplay side of the weather (saber fizz in the rain, acid rain damage, plus MP's lava weather damage) needs
// two things the MP renderers keep to themselves: which weather is active and whether a point is "outside".
// - The weather is read from the same "*..." CS_EFFECTS strings the renderers get (the game issues them, the cgame
//   receives them), so the game, the cgame and demos all agree, whatever the host's renderer or r_weather does.
// - "Outside" works like SP: the map's system/outside and system/inside weather brushes first (SP vanilla's COutside
//   is nothing but CM_PointContents on these), then, for a point in neither, a trace straight up looking for sky.
//   The collision model has the brush contents, so this works on a dedicated server (no renderer).

#include "qcommon/q_shared.h"
#include "bg_public.h"

#if defined(_GAME)
#include "g_local.h"
#elif defined(_CGAME)
#include "cgame/cg_local.h"
#endif

// how far up the sky trace looks (the BSP's world limit is 65536, maps are far smaller)
#define BG_WEATHER_SKY_TRACE_DIST	16384.0f

// chance per 50 ms of a blade fizzing, as SP vanilla's R_GetChanceOfSaberFizz (rain cloud gravity / 20000)
#define BG_WEATHER_FIZZ_RAIN		0.1f // lightrain, rain, acidrain: gravity 2000
#define BG_WEATHER_FIZZ_HEAVYRAIN	0.14f // heavyrain: gravity 2800

void BG_WeatherReset(bgWeatherState_t* ws)
{
	if (ws)
	{
		memset(ws, 0, sizeof * ws);
	}
}

static void BG_WeatherAddRain(bgWeatherState_t* ws, const float fizz_chance)
{
	ws->raining = qtrue;

	if (ws->saberFizzChance < fizz_chance)
	{
		ws->saberFizzChance = fizz_chance;
	}
}

/*
=================
BG_WeatherParseCommand

One weather command, as RE_WorldEffectCommand gets it (with or without the leading '*').
Only the commands that matter for gameplay are tracked, the rest (fog, wind, snow, ...) are ignored.
=================
*/
void BG_WeatherParseCommand(bgWeatherState_t* ws, const char* command)
{
	char token[32];
	size_t len = 0;

	if (!ws || !command)
	{
		return;
	}

	if (*command == '*')
	{
		command++;
	}

	while (*command == ' ' || *command == '\t')
	{
		command++;
	}

	while (*command && *command != ' ' && *command != '\t' && len < sizeof token - 1)
	{
		token[len++] = *command++;
	}
	token[len] = '\0';

	if (!Q_stricmp(token, "clear") || !Q_stricmp(token, "die"))
	{
		BG_WeatherReset(ws);
	}
	else if (!Q_stricmp(token, "lightrain") || !Q_stricmp(token, "rain"))
	{
		BG_WeatherAddRain(ws, BG_WEATHER_FIZZ_RAIN);
	}
	else if (!Q_stricmp(token, "heavyrain"))
	{
		BG_WeatherAddRain(ws, BG_WEATHER_FIZZ_HEAVYRAIN);
	}
	else if (!Q_stricmp(token, "acidrain"))
	{
		BG_WeatherAddRain(ws, BG_WEATHER_FIZZ_RAIN);
		ws->acidPain = qtrue;
	}
	else if (!Q_stricmp(token, "outsidepain"))
	{
		ws->acidPain = ws->acidPain ? qfalse : qtrue;
	}
	else if (!Q_stricmp(token, "lava"))
	{
		ws->lava = qtrue;
	}
}

/*
=================
BG_WeatherPresetName

The numbered weather presets: the in-game menu's weather selector (r_weather), the "r_weather N" lines of the per-map
Weather/<mapname>.cfg files and "weather <number>". Each one is the weather command that makes exactly what the
renderers' old r_weather preset N made (rd-vanilla tr_WorldEffects.cpp / rd-rend2 tr_weather.cpp).
Returns NULL for 0 (none) and for numbers that are no preset.
=================
*/
const char* BG_WeatherPresetName(const int preset)
{
	static const char* const presets[BG_WEATHER_NUM_PRESETS] = {
		NULL,			// 0: none
		"snow",			// 1
		"rain",			// 2
		"lava",			// 3
		"sand",			// 4
		"heavyrainfog",	// 5
		"fog",			// 6
		"acidrain",		// 7
		"wind",			// 8
		"spacedust",	// 9
	};

	if (preset <= 0 || preset >= BG_WEATHER_NUM_PRESETS)
	{
		return NULL;
	}
	return presets[preset];
}

/*
=================
BG_WeatherPointOutside

Gameplay "is this point under the open sky". Costs one point contents query, plus one trace when the point is in no
weather brush. Callers should cache the result (it is meant for a few checks per entity per second, not per frame).
pass_entity_num is only used by the game (the cgame traces the world only).
=================
*/
qboolean BG_WeatherPointOutside(const vec3_t point, const int pass_entity_num)
{
	trace_t tr;
	vec3_t end;

#if defined(_GAME)
	const int contents = trap->PointContents(point, pass_entity_num);
#elif defined(_CGAME)
	const int contents = trap->CM_PointContents(point, 0);
	(void)pass_entity_num;
#endif

	// in a wall, or under water / slime / lava: never outside (as SP vanilla's COutside::ContentsOutside)
	if (contents & (CONTENTS_SOLID | CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA))
	{
		return qfalse;
	}

	// then the map's weather brushes, as in the SP renderers
	if (contents & CONTENTS_INSIDE)
	{
		return qfalse;
	}
	if (contents & CONTENTS_OUTSIDE)
	{
		return qtrue;
	}

	// no weather brush here: outside if nothing solid is above the point but sky
	VectorSet(end, point[0], point[1], point[2] + BG_WEATHER_SKY_TRACE_DIST);
	memset(&tr, 0, sizeof tr);

#if defined(_GAME)
	trap->Trace(&tr, point, NULL, NULL, end, pass_entity_num, MASK_SOLID, qfalse, 0, 0);
#elif defined(_CGAME)
	trap->CM_Trace(&tr, point, end, NULL, NULL, 0, MASK_SOLID, qfalse);
#endif

	if (tr.startsolid || tr.allsolid)
	{
		return qfalse;
	}

	if (tr.fraction >= 1.0f || tr.surfaceFlags & SURF_SKY)
	{
		return qtrue;
	}

	return qfalse;
}