//ported from SP
#include "b_local.h"

#define	ASSASSIN_SHIELD_SIZE	75
#define TURN_ON					0x00000000
#define TURN_OFF				0x00000100

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
static qboolean BubbleShield_IsOn(const gentity_t* self)
{
	return self->flags & FL_SHIELDED;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
static void BubbleShield_TurnOn(gentity_t* self)
{
	if (!BubbleShield_IsOn(self))
	{
		self->flags |= FL_SHIELDED;
		NPCS.NPC->client->ps.powerups[PW_GALAK_SHIELD] = Q3_INFINITE;
		NPC_SetSurfaceOnOff(self, "force_shield", TURN_ON);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void BubbleShield_TurnOff(gentity_t* self)
{
	if (BubbleShield_IsOn(self))
	{
		self->flags &= ~FL_SHIELDED;
		NPCS.NPC->client->ps.powerups[PW_GALAK_SHIELD] = 0;
		NPC_SetSurfaceOnOff(self, "force_shield", TURN_OFF);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Push A Particular Ent
////////////////////////////////////////////////////////////////////////////////////////
static void BubbleShield_PushEnt(gentity_t* pushed, vec3_t smack_dir)
{
	G_Damage(pushed, NPCS.NPC, NPCS.NPC, smack_dir, NPCS.NPC->r.currentOrigin,
		(g_npcspskill.integer + 1) * Q_irand(5, 10), DAMAGE_NO_KNOCKBACK, MOD_ELECTROCUTE);
	G_Throw(pushed, smack_dir, 10);

	// Make Em Electric
	//------------------
	if (pushed->client)
	{
		pushed->client->ps.electrifyTime = level.time + 1000;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Go Through All The Ents Within The Radius Of The Shield And Push Them
////////////////////////////////////////////////////////////////////////////////////////
// Push away entities inside the Assassin Droid bubble shield radius.
// Behaviour preserved; only structure, safety, and stack usage improved.
static void BubbleShield_PushRadiusEnts(void)
{
	// static: too big for the stack, and BG_Alloc is never freed: asked for on every call, this ran the pool dry
	static int entity_list[MAX_GENTITIES];

	const float radius = ASSASSIN_SHIELD_SIZE;
	vec3_t mins = { 0, 0, 0 }, maxs = { 0, 0, 0 };

	// Compute bounding box around NPC
	for (int i = 0; i < 3; i++)
	{
		mins[i] = NPCS.NPC->r.currentOrigin[i] - radius;
		maxs[i] = NPCS.NPC->r.currentOrigin[i] + radius;
	}

	// Query entities inside bounding box
	const int num_ents = trap->EntitiesInBox(mins, maxs, entity_list, MAX_GENTITIES);

	for (int i = 0; i < num_ents; i++)
	{
		const int entNum = entity_list[i];

		// Validate index
		if (entNum < 0 || entNum >= MAX_GENTITIES)
		{
			continue;
		}

		gentity_t* radius_ent = &g_entities[entNum];

		// Only clients
		if (!radius_ent || !radius_ent->client)
		{
			continue;
		}

		// Do not push other Assassin Droids
		if (radius_ent->client->NPC_class == NPCS.NPC->client->NPC_class)
		{
			continue;
		}

		// Enemy already pushed by touch logic
		if (NPCS.NPC->enemy &&
			NPCS.NPCInfo->touchedByPlayer == NPCS.NPC->enemy &&
			radius_ent == NPCS.NPC->enemy)
		{
			continue;
		}

		// Compute direction from NPC to entity
		vec3_t smack_dir;
		VectorSubtract(radius_ent->r.currentOrigin, NPCS.NPC->r.currentOrigin, smack_dir);

		const float smackDist = VectorNormalize(smack_dir);

		// Inside radius → push entity
		if (smackDist < radius)
		{
			BubbleShield_PushEnt(radius_ent, smack_dir);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void bubble_shield_update(void)
{
	// Shields Go When You Die
	//-------------------------
	if (NPCS.NPC->health <= 0)
	{
		BubbleShield_TurnOff(NPCS.NPC);
		return;
	}

	// Recharge Shields
	//------------------
	NPCS.NPC->client->ps.stats[STAT_ARMOR] += 1;
	if (NPCS.NPC->client->ps.stats[STAT_ARMOR] > 250)
	{
		NPCS.NPC->client->ps.stats[STAT_ARMOR] = 250;
	}

	// If We Have Enough Armor And Are Not Shooting Right Now, Kick The Shield On
	//----------------------------------------------------------------------------
	if (NPCS.NPC->client->ps.stats[STAT_ARMOR] > 100 && TIMER_Done(NPCS.NPC, "ShieldsDown"))
	{
		// Check On Timers To Raise And Lower Shields
		//--------------------------------------------
		if (level.time - NPCS.NPCInfo->enemyLastSeenTime < 1000 && TIMER_Done(NPCS.NPC, "ShieldsUp"))
		{
			TIMER_Set(NPCS.NPC, "ShieldsDown", 2000); // Drop Shields
			TIMER_Set(NPCS.NPC, "ShieldsUp", Q_irand(4000, 5000)); // Then Bring Them Back Up For At Least 3 sec
		}

		BubbleShield_TurnOn(NPCS.NPC);
		if (BubbleShield_IsOn(NPCS.NPC))
		{
			// Update Our Shader Value
			//-------------------------
			NPCS.NPC->s.customRGBA[0] =
				NPCS.NPC->s.customRGBA[1] =
				NPCS.NPC->s.customRGBA[2] =
				NPCS.NPC->s.customRGBA[3] = NPCS.NPC->client->ps.stats[STAT_ARMOR] - 100;

			// If Touched By An Enemy, ALWAYS Shove Them
			//-------------------------------------------
			if (NPCS.NPC->enemy && NPCS.NPCInfo->touchedByPlayer == NPCS.NPC->enemy)
			{
				vec3_t dir;
				VectorSubtract(NPCS.NPC->enemy->r.currentOrigin, NPCS.NPC->r.currentOrigin, dir);
				VectorNormalize(dir);
				BubbleShield_PushEnt(NPCS.NPC->enemy, dir);
			}

			// Push Anybody Else Near
			//------------------------
			BubbleShield_PushRadiusEnts();
		}
	}
	else
	{
		BubbleShield_TurnOff(NPCS.NPC);
	}
}

// ============================================================================
// MovieDuels: the droideka's shield, ported from MD SP (AI_AssassinDroid.cpp deka_bubble_shield_update,
// BubbleShield_TurnOn / TurnOff, AI_Character.cpp droideka_npc). Unlike the assassin droid it does not shove or
// electrocute anyone and recharges to 100 each time it comes up. Only CLASS_DROIDEKA NPCs with WP_DROIDEKA use it.
// ============================================================================

// SP droideka_npc(): a droideka NPC with its own weapon (not a player or a bot)
qboolean Droideka_IsNPC(const gentity_t* ent)
{
	if (!ent || !ent->client || !ent->NPC)
	{
		return qfalse;
	}
	return ent->client->NPC_class == CLASS_DROIDEKA && ent->s.weapon == WP_DROIDEKA && ent->s.number >= MAX_CLIENTS
		? qtrue
		: qfalse;
}

// SP BubbleShield_TurnOn: not while the "ShieldsDown" cooldown runs (also set by the roll, bg_pmove.c)
void Droideka_ShieldOn(gentity_t* self)
{
	if (!self || !self->client)
	{
		return;
	}
	if (!(self->flags & FL_SHIELDED) && TIMER_Done(self, "ShieldsDown"))
	{
		self->flags |= FL_SHIELDED;
		self->client->ps.powerups[PW_GALAK_SHIELD] = Q3_INFINITE;
		NPC_SetSurfaceOnOff(self, "force_shield", TURN_ON);
	}
}

// SP BubbleShield_TurnOff
void Droideka_ShieldOff(gentity_t* self)
{
	if (!self || !self->client)
	{
		return;
	}
	if (self->flags & FL_SHIELDED)
	{
		self->flags &= ~FL_SHIELDED;
		self->client->ps.powerups[PW_GALAK_SHIELD] = 0;
		NPC_SetSurfaceOnOff(self, "force_shield", TURN_OFF);
	}
}

// SP deka_bubble_shield_update: up for 6-8 s while the enemy is in sight, then down for 2.5 s
void Droideka_ShieldUpdate(void)
{
	gentity_t* self = NPCS.NPC;

	if (!self || !self->client || !NPCS.NPCInfo)
	{
		return;
	}

	// Turn shield off if NPC is dead
	if (self->health <= 0)
	{
		Droideka_ShieldOff(self);
		return;
	}

	// Turn shield off if ShieldsUp timer is done
	if (self->flags & FL_SHIELDED && TIMER_Done(self, "ShieldsUp"))
	{
		Droideka_ShieldOff(self);
		TIMER_Set(self, "ShieldsDown", 2500); // 2.5 second cooldown
		return;
	}

	// Turn shield OFF if shield health is 0
	if (self->flags & FL_SHIELDED && self->client->ps.stats[STAT_ARMOR] <= 0)
	{
		Droideka_ShieldOff(self);
		TIMER_Set(self, "ShieldsDown", 2500); // 2.5 second cooldown
		return;
	}

	// Turn shield on at full health once ShieldsDown timer is done
	if (!(self->flags & FL_SHIELDED) && TIMER_Done(self, "ShieldsDown")
		&& level.time - NPCS.NPCInfo->enemyLastSeenTime < 1000)
	{
		self->client->ps.stats[STAT_ARMOR] = 100;
		Droideka_ShieldOn(self);
		TIMER_Set(self, "ShieldsUp", Q_irand(6000, 8000)); // Shield stays up for 6-8 seconds

		// Update shader value
		self->s.customRGBA[0] =
			self->s.customRGBA[1] =
			self->s.customRGBA[2] =
			self->s.customRGBA[3] = self->client->ps.stats[STAT_ARMOR];
	}
}
