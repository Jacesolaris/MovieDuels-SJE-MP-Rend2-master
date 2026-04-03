#ifndef CG_SAFEGUARDS_H
#define CG_SAFEGUARDS_H

#include "cg_local.h"
#include <game/bg_public.h>

#define MAX_SHOWPOWERS NUM_FORCE_POWERS
#define VALID_FORCE_INDEX(i) ((i) >= 0 && (i) < MAX_SHOWPOWERS)
#define VALID_ITEM_INDEX(i)  ((i) >= 0 && (i) < HI_NUM_HOLDABLE)

static inline const char* GetShowPowerNameSafe(int idx)
{
    if (!VALID_FORCE_INDEX(idx)) return NULL;
    extern char* showPowersName[]; // defined in cg_draw.c
    return showPowersName[idx];
}

static inline qhandle_t GetForceIconSafe(int idx)
{
    if (!VALID_FORCE_INDEX(idx)) return 0;
    extern qhandle_t* forcePowerIcons; // cgs.media.forcePowerIcons
    return cgs.media.forcePowerIcons[idx];
}

static inline qhandle_t GetInvenIconSafe(int idx)
{
    if (!VALID_ITEM_INDEX(idx)) return 0;
    return cgs.media.invenIcons[idx];
}

static inline int GetBgItemIndexSafe(int tag)
{
    const int itemIndex = BG_GetItemIndexByTag(tag, IT_HOLDABLE);
    if (itemIndex < 0 || itemIndex >= HI_NUM_HOLDABLE) return -1;
    return itemIndex;
}

#endif // CG_SAFEGUARDS_H
