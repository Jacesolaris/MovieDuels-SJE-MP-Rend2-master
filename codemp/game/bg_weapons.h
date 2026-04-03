/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
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

#pragma once

// Filename:-	bg_weapons.h
//
// This crosses both client and server.  It could all be crammed into bg_public, but isolation of this type of data is best.

typedef enum
{
	WP_NONE,            // selectable never in the game, not even with give all

	WP_STUN_BATON,       // selectable only for BCLASS_JAWA unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_MELEE,            // selectable for all classes.
	WP_SABER,            // selectable only if Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_BRYAR_PISTOL,     // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_BLASTER,          // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_DISRUPTOR,        // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_BOWCASTER,        // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_REPEATER,         // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_DEMP2,            // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_FLECHETTE,        // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_ROCKET_LAUNCHER,  // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_THERMAL,          // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_TRIP_MINE,        // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_DET_PACK,         // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_CONCUSSION,       // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD

	// new guns
	WP_BATTLEDROID,      // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_THEFIRSTORDER,    // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_CLONECARBINE,     // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_REBELBLASTER,     // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_CLONERIFLE,       // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_CLONECOMMANDO,    // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_REBELRIFLE,       // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_REY,              // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_JANGO,            // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_BOBA,             // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	WP_CLONEPISTOL,      // selectable for all classes except BCLASS_SBD AND Bot_Is_Saber_Class(ent) unless cheats are on, then it is selectable for all classes except BCLASS_SBD
	//end of new guns

	WP_BRYAR_OLD,        // Only BCLASS_SBD can select this from WP_MELEE and back to WP_MELEE if cycling weapons
	WP_EMPLACED_GUN,     // Not selectable in any cases even with cheats on
	WP_TURRET,           // Not selectable in any cases even with cheats on

	WP_NUM_WEAPONS
} weapon_t;

#define FIRST_SELECTABLE_WEAPON		WP_STUN_BATON  // this is the first weapon for next and prev weapon switching
#define LAST_SELECTABLE_WEAPON	    WP_BRYAR_OLD   //anything > this will be considered not player useable

#define WP_MELEEONLY		524283
#define WP_SABERSONLY		524279
#define WP_MELEESABERS		524275
#define WP_NOEXPLOS			28672

#define WP_ALLDISABLED		524287
#define FP_ALLDISABLED		262143

typedef enum //# ammo_e
{
	AMMO_NONE,
	AMMO_FORCE,
	// AMMO_PHASER
	AMMO_BLASTER,
	// AMMO_STARFLEET,
	AMMO_POWERCELL,
	// AMMO_ALIEN,
	AMMO_METAL_BOLTS,
	AMMO_ROCKETS,
	AMMO_EMPLACED,
	AMMO_THERMAL,
	AMMO_TRIPMINE,
	AMMO_DETPACK,
	AMMO_MAX
} ammo_t;

typedef struct weaponData_s
{
	//	char	classname[32];		// Spawning name

	int ammoIndex; // Index to proper ammo slot
	int ammoLow; // Count when ammo is low

	int energyPerShot; // Amount of energy used per shot
	int fireTime; // Amount of time between firings
	int range; // Range of weapon

	int altEnergyPerShot; // Amount of energy used for alt-fire
	int altFireTime; // Amount of time between alt-firings
	int altRange; // Range of alt-fire

	int chargeSubTime; // ms interval for subtracting ammo during charge
	int altChargeSubTime; // above for secondary

	int chargeSub; // amount to subtract during charge on each interval
	int altChargeSub; // above for secondary

	int maxCharge; // stop subtracting once charged for this many ms
	int altMaxCharge; // above for secondary
} weaponData_t;

typedef struct ammoData_s
{
	//	char	icon[32];	// Name of ammo icon file
	int max; // Max amount player can hold of ammo
} ammoData_t;

extern weaponData_t weaponData[WP_NUM_WEAPONS];
extern ammoData_t ammoData[AMMO_MAX];

// Specific weapon information

#define DEFAULT_SHOTGUN_SPREAD	700
#define DEFAULT_SHOTGUN_COUNT	11

#define	LIGHTNING_RANGE		768
