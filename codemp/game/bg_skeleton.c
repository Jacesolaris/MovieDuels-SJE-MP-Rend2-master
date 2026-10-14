// bg_skeleton.c -- MovieDuels MP: player model skeleton checks, shared by game, cgame and ui.
//
// SJE->MD parity (2026-10-13): moved unchanged from bg_panimate.c (BG_ModelSkeletonFlags, BG_ClassIsCreature,
// BG_ClassSkeletonOk and their glm/gla file readers), as SJE MP has them in its bg_skeleton.c, so the ui's character menu
// (MD_CharModelSupported) decides "playable" with exactly the rules the server (G_AssignClassAndScaleFromModel) and the
// client (CG_RegisterClientModelname) use. New here: BG_IsHumanoidFamilyPath (the humanoid_prefixes list, also used by
// bg_panimate.c and bg_misc.c BG_IsOwnSkeletonModel) and BG_ModelUsesHumanoidGLA (SJE's no-usable-skeleton test).

#include "qcommon/q_shared.h"
#include "bg_public.h"
#include "anims.h"
#include "teams.h"

#ifdef _GAME
#include "g_local.h"
#elif _CGAME
#include "cgame/cg_local.h"
#elif UI_BUILD
#include "ui/ui_local.h"
#endif

/*
======================
glm / gla file readers (the same as bg_panimate.c's, which keeps its own for the anim set skeleton info)
======================
*/
#define BG_GLA_SKEL_READ_SIZE	65536	// enough for the gla header and skeleton (bone names)
#define BG_GLA_HEADER_SIZE		100
#define BG_GLA_MAX_BONES		1024

static byte bgGLASkelBuf[BG_GLA_SKEL_READ_SIZE];

static int BG_ReadFileStart(const char* path, byte* buf, const int bufSize)
{
	fileHandle_t f = 0;
	const int len = trap->FS_Open(path, &f, FS_READ);

	if (!f)
	{
		return 0;
	}
	if (len <= 0)
	{
		trap->FS_Close(f);
		return 0;
	}

	const int n = len < bufSize ? len : bufSize;
	trap->FS_Read(buf, n, f);
	trap->FS_Close(f);
	return n;
}

static int BG_ReadFileInt(const byte* buf, const int len, const int ofs)
{
	int v = 0;

	if (ofs < 0 || ofs + (int)sizeof v > len)
	{
		return -1;
	}
	memcpy(&v, buf + ofs, sizeof v);
	return LittleLong(v);
}

// gla (mdxaHeader_t): numBones at 84; the bone offset table follows the 100 byte header, each bone starts with its name
static qboolean BG_GLAHasBone(const byte* buf, const int len, const char* boneName)
{
	if (len < BG_GLA_HEADER_SIZE || memcmp(buf, "2LGA", 4) != 0)
	{
		return qfalse;
	}

	const int numBones = BG_ReadFileInt(buf, len, 84);
	if (numBones <= 0 || numBones > BG_GLA_MAX_BONES)
	{
		return qfalse;
	}

	for (int i = 0; i < numBones; i++)
	{
		const int boneOfs = BG_ReadFileInt(buf, len, BG_GLA_HEADER_SIZE + i * 4);
		const int nameOfs = BG_GLA_HEADER_SIZE + boneOfs;
		if (boneOfs < 0 || nameOfs + MAX_QPATH > len)
		{
			return qfalse;
		}
		if (memchr(buf + nameOfs, 0, MAX_QPATH) && !Q_stricmp((const char*)(buf + nameOfs), boneName))
		{
			return qtrue;
		}
	}
	return qfalse;
}

/*
======================
The humanoid-family folders (the renderers' humanoid_prefixes, tr_model.cpp R_MD_ChooseAnimGLA): the master and the
dedicated _humanoid_* sets and the humanoid-rigged NPC skeletons. A path in one of them, own skeleton or not.
======================
*/
static const char* const bgHumanoidPrefixes[] =
{
	"models/players/_humanoid",
	"models/players/JK2anims",
	"models/players/_humanoid_ani",
	"models/players/_humanoid_bdroid",
	"models/players/_humanoid_ben",
	"models/players/_humanoid_cal",
	"models/players/_humanoid_clo",
	"models/players/_humanoid_deka",
	"models/players/_humanoid_df2",
	"models/players/_humanoid_dooku",
	"models/players/_humanoid_galen",
	"models/players/_humanoid_gon",
	"models/players/_humanoid_grievous",
	"models/players/_humanoid_jabba",
	"models/players/_humanoid_jango",
	"models/players/_humanoid_kotor",
	"models/players/_humanoid_luke",
	"models/players/_humanoid_mace",
	"models/players/_humanoid_maul",
	"models/players/_humanoid_md",
	"models/players/_humanoid_melee",
	"models/players/_humanoid_obi",
	"models/players/_humanoid_obi3",
	"models/players/_humanoid_pal",
	"models/players/_humanoid_reb",
	"models/players/_humanoid_ren",
	"models/players/_humanoid_rey",
	"models/players/_humanoid_sbd",
	"models/players/_humanoid_vader",
	"models/players/_humanoid_yoda",
	"models/players/darktrooper_tv",
	"models/players/hazardtrooper/hazardtrooper",
	"models/players/rockettrooper/rockettrooper",
	"models/players/protocol/protocol",
	"models/players/assassin_droid/model",
	"models/players/saber_droid/model",
	"models/players/wampa/wampa"
};

qboolean BG_IsHumanoidFamilyPath(const char* path)
{
	if (!path || !path[0])
	{
		return qfalse;
	}
	for (int i = 0; i < (int)ARRAY_LEN(bgHumanoidPrefixes); i++)
	{
		if (strstr(path, bgHumanoidPrefixes[i]) == path)
		{
			return qtrue;
		}
	}
	return qfalse;
}

/*
======================
MovieDuels (port of SJE MP's creature work): creature and droid player models

MD's own-skeleton rule (BG_IsOwnSkeletonModel: the model.glm's bone count is neither the master's nor 72 - or, SJE->MD
parity 2026-10-13, its GLA is outside every humanoid-family folder - and the GLA's folder has an animation.cfg) decides which models keep their own skeleton. For those, BG_ModelSkeletonFlags says which
player bolts/bones the skeleton has (SKELF_*). SKELF_FULL = every bolt/bone the humanoid player code needs (the old,
fixed cgame test). A skeleton without them (r2d2, wampa, rancor, protocol, howler...) is only playable as a creature
class that needs nothing the skeleton lacks (BG_ClassSkeletonOk). The server (G_AssignClassAndScaleFromModel) and the
client (CG_RegisterClientModelname) both use this, so both sides agree.
======================
*/
#define BG_SKEL_GLM_READ_SIZE	131072	// enough for the glm header and its surface hierarchy
#define BG_SKEL_GLM_HEADER_SIZE	164		// mdxmHeader_t
#define BG_SKEL_FLAGS_CACHE		64

static byte bgGLMSkelBuf[BG_SKEL_GLM_READ_SIZE];

// a name stored in a fixed char[64] (MAX_QPATH) field of the file, bounds checked
static const char* BG_ReadFileName(const byte* buf, const int len, const int ofs)
{
	if (ofs < 0 || ofs + MAX_QPATH > len)
	{
		return NULL;
	}
	if (!memchr(buf + ofs, 0, MAX_QPATH))
	{
		return NULL;
	}
	return (const char*)(buf + ofs);
}

// glm (mdxmHeader_t): numSurfaces at 152, ofsSurfHierarchy at 156; each mdxmSurfHierarchy_t is
// name[64] flags shader[64] shaderIndex parentIndex numChildren childIndexes[numChildren]
static qboolean BG_GLMHasSurface(const byte* buf, const int len, const char* surfName)
{
	if (len < BG_SKEL_GLM_HEADER_SIZE || memcmp(buf, "2LGM", 4) != 0)
	{
		return qfalse;
	}

	const int numSurfaces = BG_ReadFileInt(buf, len, 152);
	int ofs = BG_ReadFileInt(buf, len, 156);
	if (numSurfaces <= 0 || numSurfaces > BG_GLA_MAX_BONES || ofs < BG_SKEL_GLM_HEADER_SIZE)
	{
		return qfalse;
	}

	for (int i = 0; i < numSurfaces; i++)
	{
		const char* name = BG_ReadFileName(buf, len, ofs);
		const int numChildren = BG_ReadFileInt(buf, len, ofs + MAX_QPATH + 4 + MAX_QPATH + 4 + 4);
		if (!name || numChildren < 0 || numChildren > BG_GLA_MAX_BONES)
		{
			return qfalse;
		}
		if (!Q_stricmp(name, surfName))
		{
			return qtrue;
		}
		ofs += MAX_QPATH + 4 + MAX_QPATH + 4 + 4 + 4 + numChildren * 4;
	}
	return qfalse;
}

// G2API_AddBolt looks for a surface (tag) of that name first, then a bone
static qboolean BG_SkelHasBolt(const int glmLen, const int glaLen, const char* name)
{
	return BG_GLMHasSurface(bgGLMSkelBuf, glmLen, name) || BG_GLAHasBone(bgGLASkelBuf, glaLen, name) ? qtrue : qfalse;
}

static int BG_ModelSkeletonCheck(const char* folder)
{
	char path[MAX_QPATH];
	char gla[MAX_QPATH];
	int flags = SKELF_OWN;

	if (!BG_IsOwnSkeletonModel(folder))
	{
		return 0; // the master skeleton (or a dedicated 53-bone humanoid one): the normal path
	}

	Com_sprintf(path, sizeof path, "models/players/%s/model.glm", folder);
	const int glmLen = BG_ReadFileStart(path, bgGLMSkelBuf, BG_SKEL_GLM_READ_SIZE);
	const char* animName = BG_ReadFileName(bgGLMSkelBuf, glmLen, 72);
	if (glmLen < BG_SKEL_GLM_HEADER_SIZE || memcmp(bgGLMSkelBuf, "2LGM", 4) != 0 || !animName || !animName[0])
	{
		return 0;
	}
	Q_strncpyz(gla, animName, sizeof gla);

	Com_sprintf(path, sizeof path, "%s.gla", gla);
	const int glaLen = BG_ReadFileStart(path, bgGLASkelBuf, BG_GLA_SKEL_READ_SIZE);
	if (glaLen <= 0 || !BG_GLAHasBone(bgGLASkelBuf, glaLen, "model_root"))
	{
		return 0; // the renderer's model_root bone anim would fail: not usable by a player at all
	}

	if (BG_SkelHasBolt(glmLen, glaLen, "*r_hand"))
	{
		flags |= SKELF_RHAND;
	}
	if (BG_SkelHasBolt(glmLen, glaLen, "*l_hand"))
	{
		flags |= SKELF_LHAND;
	}
	const qboolean headTop = BG_SkelHasBolt(glmLen, glaLen, "*head_top") || BG_SkelHasBolt(glmLen, glaLen, "ceyebrow")
		? qtrue : qfalse;
	if (headTop || BG_SkelHasBolt(glmLen, glaLen, "*head_t") || BG_SkelHasBolt(glmLen, glaLen, "*head_eyes"))
	{
		flags |= SKELF_HEAD;
	}
	if (BG_SkelHasBolt(glmLen, glaLen, "Motion"))
	{
		flags |= SKELF_MOTION;
	}
	if (BG_SkelHasBolt(glmLen, glaLen, "lower_lumbar"))
	{
		flags |= SKELF_LLUMBAR;
	}
	if (BG_GLAHasBone(bgGLASkelBuf, glaLen, "upper_lumbar"))
	{
		flags |= SKELF_ULUMBAR;
	}
	if (BG_GLAHasBone(bgGLASkelBuf, glaLen, "cranium"))
	{
		flags |= SKELF_CRANIUM;
	}

	// the old fixed cgame test: bolts *r_hand, *l_hand, *head_top (or ceyebrow), Motion, lower_lumbar; bones model_root,
	// upper_lumbar, cranium
	if (headTop &&
		(flags & (SKELF_RHAND | SKELF_LHAND | SKELF_MOTION | SKELF_LLUMBAR | SKELF_ULUMBAR | SKELF_CRANIUM)) ==
		(SKELF_RHAND | SKELF_LHAND | SKELF_MOTION | SKELF_LLUMBAR | SKELF_ULUMBAR | SKELF_CRANIUM))
	{
		flags |= SKELF_FULL;
	}
	return flags;
}

// The SKELF_* of a userinfo model string's own skeleton ("folder", "folder/skin", "folder/|head|torso|lower"), 0 when it
// uses the master (or a dedicated humanoid) skeleton or its skeleton cannot be used by a player. Cached per folder.
int BG_ModelSkeletonFlags(const char* model)
{
	static struct
	{
		char	folder[MAX_QPATH];
		int		flags;
	} cache[BG_SKEL_FLAGS_CACHE];
	static int numCached = 0;
	char folder[MAX_QPATH];
	int n = 0;

	if (!model || !model[0])
	{
		return 0;
	}
	for (const char* s = model; *s && *s != '/' && *s != '\\' && *s != '|' && n < (int)sizeof folder - 1; s++)
	{
		const char c = *s;
		if (Q_IsColorString(s))
		{
			s++;
			continue;
		}
		folder[n++] = c;
	}
	folder[n] = 0;
	if (!folder[0])
	{
		return 0;
	}

	for (int i = 0; i < numCached; i++)
	{
		if (!Q_stricmp(cache[i].folder, folder))
		{
			return cache[i].flags;
		}
	}

	const int flags = BG_ModelSkeletonCheck(folder);
	if (numCached < BG_SKEL_FLAGS_CACHE)
	{
		Q_strncpyz(cache[numCached].folder, folder, sizeof cache[numCached].folder);
		cache[numCached].flags = flags;
		numCached++;
	}
	return flags;
}

// ---------------------------------------------------------------------------------------------------------------
// Creature and droid player classes. Values from the SP NPC files (ext_data/npcs/*.npc) and the SP class code.
// ---------------------------------------------------------------------------------------------------------------
qboolean BG_ClassIsCreature(const int botclass)
{
	switch (botclass)
	{
	case BCLASS_R2D2:
	case BCLASS_R5D2:
	case BCLASS_GONK:
	case BCLASS_PROTOCOL:
	case BCLASS_WAMPA:
	case BCLASS_RANCOR:
	case BCLASS_HOWLER:
	case BCLASS_MINEMONSTER:
	case BCLASS_ASSASSIN_DROID:
	case BCLASS_HAZARDTROOPER:
	case BCLASS_SABER_DROID:
		return qtrue;
	default:
		return qfalse;
	}
}

// Every skeleton with the full humanoid bolts is fine for any class (as before). A creature skeleton only for a creature
// class, and only with the bolts that class needs: a hand for a hand-held weapon (the weapon model sits on *r_hand, the
// saber on *r_hand / *l_hand). Melee and the stun baton hit from the view origin, so they need no hand.
qboolean BG_ClassSkeletonOk(const int botclass, const int skelFlags)
{
	if (!(skelFlags & SKELF_OWN) || skelFlags & SKELF_FULL)
	{
		return qtrue;
	}
	if (botclass == BCLASS_ELDER)
	{
		// melee only, no Force, no saber (jabba: both hands and a head, no Motion / upper_lumbar)
		return (skelFlags & (SKELF_RHAND | SKELF_LHAND | SKELF_HEAD)) == (SKELF_RHAND | SKELF_LHAND | SKELF_HEAD) ? qtrue : qfalse;
	}
	if (!BG_ClassIsCreature(botclass))
	{
		return qfalse;
	}
	switch (botclass)
	{
	case BCLASS_ASSASSIN_DROID:
	case BCLASS_HAZARDTROOPER:
		return skelFlags & SKELF_RHAND ? qtrue : qfalse;
	case BCLASS_SABER_DROID:
		return (skelFlags & (SKELF_RHAND | SKELF_LHAND)) == (SKELF_RHAND | SKELF_LHAND) ? qtrue : qfalse;
	default:
		return qtrue;
	}
}

// SJE->MD parity (2026-10-13), SJE's test: does the model's glm stay on a humanoid set (a humanoid-family GLA with a mesh
// that fits the master skeleton, BG_IsHumanoidFamilyPath / BG_BoneCountIsOwnSkeleton)? A model with neither that nor a
// usable own skeleton (BG_ModelSkeletonFlags: props, vehicles, a GLA without animation.cfg...) cannot be played: the
// server resets it to the default model, the client draws the default model, the menu leaves it out.
qboolean BG_ModelUsesHumanoidGLA(const char* model)
{
	char folder[MAX_QPATH];
	char path[MAX_QPATH];
	int n = 0;

	if (!model || !model[0])
	{
		return qfalse;
	}
	for (const char* s = model; *s && *s != '/' && *s != '\\' && *s != '|' && n < (int)sizeof folder - 1; s++)
	{
		const char c = *s;
		if (Q_IsColorString(s))
		{
			s++;
			continue;
		}
		folder[n++] = c;
	}
	folder[n] = 0;
	if (!folder[0])
	{
		return qfalse;
	}

	Com_sprintf(path, sizeof path, "models/players/%s/model.glm", folder);
	const int glmLen = BG_ReadFileStart(path, bgGLMSkelBuf, BG_SKEL_GLM_HEADER_SIZE);
	const char* animName = BG_ReadFileName(bgGLMSkelBuf, glmLen, 72);
	if (glmLen < BG_SKEL_GLM_HEADER_SIZE || memcmp(bgGLMSkelBuf, "2LGM", 4) != 0 || !animName)
	{
		return qfalse;
	}
	return BG_IsHumanoidFamilyPath(animName) && !BG_BoneCountIsOwnSkeleton(BG_ReadFileInt(bgGLMSkelBuf, glmLen, 140))
		? qtrue : qfalse;
}
