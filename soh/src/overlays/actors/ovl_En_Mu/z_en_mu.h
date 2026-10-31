#ifndef Z_EN_MU_H
#define Z_EN_MU_H

#include <libultraship/libultra.h>
#include "global.h"

struct EnMu;

typedef void (*EnMuActionFunc)(struct EnMu*, struct PlayState*);

typedef struct EnMu {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ EnMuActionFunc actionFunc;
    /* 0x0194 */ ColliderCylinder collider;
    /* 0x01E0 */ NpcInteractInfo npcInfo;
    /* 0x0208 */ u16 defFaceReaction;
    /* 0x020A */ union { s16 unk_20A[16]; s16 fidgetTableY[16]; };
    /* 0x022A */ union { s16 unk_22A[17]; s16 fidgetTableZ[17]; };
} EnMu; // size = 0x024C

#endif
