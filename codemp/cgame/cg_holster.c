#include "cg_local.h"
#include <qcommon/qfiles.h>
#include <qcommon/q_shared.h>
#include <stdio.h>
#include "cg_holster.h"
#include <qcommon\q_math.h>
#include <qcommon\q_string.h>

extern int BG_SiegeGetPairedValue(const char* buf, char* key, char* outbuf);
extern int BG_SiegeGetValueGroup(const char* buf, char* group, char* outbuf);

stringID_table_t holsterTypeTable[] =
{
	ENUM2STRING(HLR_NONE),
	ENUM2STRING(HLR_SINGLESABER_1), //first single saber
	ENUM2STRING(HLR_SINGLESABER_2), //second single saber
	ENUM2STRING(HLR_STAFFSABER), //staff saber
	ENUM2STRING(HLR_PISTOL_L), //left hip blaster pistol
	ENUM2STRING(HLR_PISTOL_R), //right hip blaster pistol
	ENUM2STRING(HLR_BLASTER_L), //left hip blaster rifle
	ENUM2STRING(HLR_BLASTER_R), //right hip blaster rifle
	ENUM2STRING(HLR_BRYARPISTOL_L), //left hip bryer pistol
	ENUM2STRING(HLR_BRYARPISTOL_R), //right hip bryer pistol
	ENUM2STRING(HLR_BOWCASTER), //bowcaster
	ENUM2STRING(HLR_ROCKET_LAUNCHER), //rocket launcher
	ENUM2STRING(HLR_DEMP2), //demp2
	ENUM2STRING(HLR_CONCUSSION), //concussion
	ENUM2STRING(HLR_REPEATER), //repeater
	ENUM2STRING(HLR_FLECHETTE), //flechette
	ENUM2STRING(HLR_DISRUPTOR), //disruptor
	ENUM2STRING(MAX_HOLSTER),

	{ NULL, 0 }   // <-- REQUIRED TERMINATOR
};

stringID_table_t holsterBoneTable[] =
{
	ENUM2STRING(HOLSTER_NONE),
	ENUM2STRING(HOLSTER_UPPERBACK),
	ENUM2STRING(HOLSTER_LOWERBACK),
	ENUM2STRING(HOLSTER_LEFTHIP),
	ENUM2STRING(HOLSTER_RIGHTHIP)
};

static void InitHolsterData(clientInfo_t* ci)
{
	//initialize holster data with the premade defaults.
	for (int i = 0; i < MAX_HOLSTER; i++)
	{
		ci->holsterData[i].boneIndex = HOLSTER_NONE;
		VectorCopy(vec3_origin, ci->holsterData[i].posOffset);
		VectorCopy(vec3_origin, ci->holsterData[i].angOffset);
	}
}

extern char* BG_GetNextValueGroup(char* inbuf, char* outbuf);

/*
==============================
CG_LoadHolsterData

Loads holster offset data from holster_mp.cfg or holster_mp<skin>.cfg
and applies it to the clientInfo_t structure.

- Moves large buffers off the stack
- Checks sscanf() return values (fixes C6031)
- Preserves original behaviour
==============================
*/
void CG_LoadHolsterData(clientInfo_t* ci)
{
	fileHandle_t f;
	int f_len;

	/* Large buffers moved off stack */
	static char file_buffer[MAX_HOLSTER_INFO_SIZE];
	static char holster_type_group[MAX_HOLSTER_INFO_SIZE];

	InitHolsterData(ci);

	/* Determine which holster file to load */
	if (ci->skinName == NULL || !Q_stricmp("default", ci->skinName))
	{
		f_len = trap->FS_Open(va("models/players/%s/holster_mp.cfg", ci->modelName), &f, FS_READ);

		if (!f)
		{
			f_len = trap->FS_Open("models/players/kyle/holster_mp.cfg", &f, FS_READ);
		}
	}
	else
	{
		f_len = trap->FS_Open(va("models/players/%s/holster_mp%s.cfg",
			ci->modelName, ci->skinName), &f, FS_READ);

		if (!f)
		{
			f_len = trap->FS_Open(va("models/players/%s/holster_mp.cfg", ci->modelName), &f, FS_READ);
		}

		if (!f)
		{
			f_len = trap->FS_Open("models/players/kyle/holster_mp.cfg", &f, FS_READ);
		}
	}

	/* If no file or empty file, nothing to load */
	if (!f || f_len <= 0)
	{
		return;
	}

	/* Prevent overflow */
	if (f_len >= MAX_HOLSTER_INFO_SIZE)
	{
		trap->FS_Close(f);
		return;
	}

	/* Read file */
	trap->FS_Read(file_buffer, f_len, f);
	trap->FS_Close(f);

	file_buffer[f_len] = '\0';

	char* s = file_buffer;

	/* Parse each holster group */
	while ((s = BG_GetNextValueGroup(s, holster_type_group)) != NULL)
	{
		vec3_t vector_data = { 0.0f, 0.0f, 0.0f };
		char holster_type_value[MAX_QPATH];

		/* holsterType */
		if (!BG_SiegeGetPairedValue(holster_type_group, "holsterType", holster_type_value))
		{
			continue;
		}

		const int i = GetIDForString(holsterTypeTable, holster_type_value);
		if (i == -1)
		{
			continue;
		}

		/* boneIndex */
		if (BG_SiegeGetPairedValue(holster_type_group, "boneIndex", holster_type_value))
		{
			if (!Q_stricmp(holster_type_value, "disabled"))
			{
				ci->holsterData[i].boneIndex = HOLSTER_NONE;
			}
			else
			{
				ci->holsterData[i].boneIndex =
					GetIDForString(holsterBoneTable, holster_type_value);
			}
		}

		/* posOffset */
		if (BG_SiegeGetPairedValue(holster_type_group, "posOffset", holster_type_value))
		{
			/* FIX: check sscanf return value */
			const int count = sscanf(holster_type_value, "%f, %f, %f",
				&vector_data[0], &vector_data[1], &vector_data[2]);

			if (count == 3)
			{
				VectorCopy(vector_data, ci->holsterData[i].posOffset);
			}
		}

		/* angOffset */
		if (BG_SiegeGetPairedValue(holster_type_group, "angOffset", holster_type_value))
		{
			/* FIX: check sscanf return value */
			const int count = sscanf(holster_type_value, "%f, %f, %f",
				&vector_data[0], &vector_data[1], &vector_data[2]);

			if (count == 3)
			{
				VectorCopy(vector_data, ci->holsterData[i].angOffset);
			}
		}
	}

#ifdef _DEBUG
	Com_Printf("Holstered Weapon Data Loaded for %s.\n", ci->modelName);
#endif
}