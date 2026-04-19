// cg_pazaak.h
#ifndef CG_PAZAAK_H
#define CG_PAZAAK_H

#include "cg_local.h"

// server command handler
void CG_Pazaak_ServerCmd_f(void);

// ownerdraw renderer
void CG_Pazaak_OwnerDraw(int ownerDraw, float x, float y, float w, float h, vec4_t color, qhandle_t shader, float scale);

// initialize client state
void CG_Pazaak_Init(void);

// Provide a fallback ownerdraw base if UI-specific header doesn't define it
#ifndef UI_JKG_PAZAAK_BASE
#define UI_JKG_PAZAAK_BASE 1000
#endif

#endif // CG_PAZAAK_H
