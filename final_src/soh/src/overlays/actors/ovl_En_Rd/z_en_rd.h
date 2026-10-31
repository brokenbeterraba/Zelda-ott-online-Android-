#ifndef Z_EN_RD_H
#define Z_EN_RD_H

#include <libultraship/libultra.h>
#include "global.h"

struct EnRd;

typedef void (*EnRdActionFunc)(struct EnRd*, PlayState*);

typedef struct EnRd {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ Vec3s firePos[10];
    /* 0x0188 */ SkelAnime skelAnime;
    /* 0x01CC */ Vec3s jointTable[26];
    /* 0x0268 */ Vec3s morphTable[26];
    /* 0x0304 */ union { u8 unk_304; u8 grabState; };
    /* 0x0305 */ union { u8 unk_305; u8 isMourning; };
    /* 0x0306 */ union { u8 unk_306; u8 playerStunWaitTimer; };
    /* 0x0307 */ union { u8 unk_307; u8 grabWaitTimer; };
    /* 0x0308 */ EnRdActionFunc actionFunc;
    /* 0x030C */ union { s16 unk_30C; s16 timer; };
    /* 0x030E */ union { s16 unk_30E; s16 headYRotation; };
    /* 0x0310 */ union { s16 unk_310; s16 upperBodyYRotation; };
    /* 0x0312 */ union { s16 unk_312; s16 rdFlags; };
    /* 0x0314 */ union { s16 unk_314; s16 alpha; };
    /* 0x0316 */ union { s16 unk_316; s16 sunsSongStunTimer; };
    /* 0x0318 */ union { u8 unk_318; u8 stunnedBySunsSong; };
    /* 0x0319 */ union { u8 unk_319; u8 grabDamageTimer; };
    /* 0x031A */ union { u8 unk_31A; u8 fireTimer; };
    /* 0x031B */ union { u8 unk_31B; u8 action; };
    /* 0x031C */ union { u8 unk_31C; u8 damageReaction; };
    /* 0x031D */ u8 unk_31D;
    /* 0x0320 */ ColliderCylinder collider;
} EnRd; // size = 0x036C

#endif
