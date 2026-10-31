#ifndef Z_EN_SYATEKI_NIW_H
#define Z_EN_SYATEKI_NIW_H

#include <libultraship/libultra.h>
#include "global.h"

struct EnSyatekiNiw;

typedef void (*EnSyatekiNiwActionFunc)(struct EnSyatekiNiw*, PlayState*);

typedef struct {
    /* 0x00 */ union { u8 unk_00; u8 state; };
    /* 0x0C */ union { Vec3f unk_04; Vec3f pos; };
    /* 0x10 */ union { Vec3f unk_10; Vec3f vel; };
    /* 0x1C */ union { Vec3f unk_1C; Vec3f accel; };
    /* 0x28 */ union { s16 unk_28; s16 lifespan; };
    /* 0x2A */ union { s16 unk_2A; s16 rotPulse; };
    /* 0x2C */ union { f32 unk_2C; f32 scale; };
    /* 0x30 */ union { f32 unk_30; f32 rot; };
    /* 0x34 */ union { u8 unk_34; u8 timer; };
    u32 epoch;
} EnSyatekiNiw_1; // size = 0x38

typedef struct EnSyatekiNiw {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ Vec3s jointTable[16];
    /* 0x01F0 */ Vec3s morphTable[16];
    /* 0x0250 */ EnSyatekiNiwActionFunc actionFunc;
    /* 0x0254 */ union { s16 unk_254; s16 peckTimer; };
    /* 0x0256 */ union { s16 unk_256; s16 timer1; };
    /* 0x0258 */ union { s16 unk_258; s16 flapTimer; };
    /* 0x025A */ union { s16 unk_25A; s16 archeryTimer; };
    /* 0x025C */ union { s16 unk_25C; s16 hopTimer; };
    /* 0x025E */ union { s16 unk_25E; s16 movementTimer; };
    /* 0x0260 */ union { s16 unk_260; s16 sootTimer; };
    /* 0x0262 */ union { s16 unk_262; s16 cluckTimer; };
    /* 0x0264 */ union { f32 unk_264; f32 headRotXTarget; };
    /* 0x0268 */ union { f32 unk_268; f32 rightWingRotXTarget; };
    /* 0x026C */ union { f32 unk_26C; f32 leftWingRotXTarget; };
    /* 0x0270 */ char unk_270[0x8];
    /* 0x0278 */ union { f32 unk_278; f32 rightWingRotYTarget; };
    /* 0x027C */ union { f32 unk_27C; f32 rightWingRotZTarget; };
    /* 0x0284 */ union { f32 unk_280; f32 leftWingRotYTarget; };
    /* 0x0280 */ union { f32 unk_284; f32 leftWingRotZTarget; };
    /* 0x0288 */ union { f32 unk_288; f32 unkArcheryFloat; };
    /* 0x028C */ union { s16 unk_28C; s16 lifetime; };
    /* 0x028E */ union { s16 unk_28E; s16 headRotXState; };
    /* 0x0290 */ s16 unk_290;
    /* 0x0292 */ union { s16 unk_292; s16 wingsRotState; };
    /* 0x0294 */ union { s16 unk_294; s16 targetPosTimer; };
    /* 0x0296 */ union { s16 unk_296; s16 archeryAnimationType; };
    /* 0x0298 */ union { s16 unk_298; s16 rotYFlip; };
    /* 0x029C */ s16 unk_29A;
    /* 0x029C */ union { s16 unk_29C; s16 archeryState; };
    /* 0x029E */ union { s16 unk_29E; s16 minigameType; };
    /* 0x02A0 */ union { s16 unk_2A0; s16 spawnFeathers; };
    /* 0x02A4 */ union { Vec3f unk_2A4; Vec3f leftWingRot; };
    /* 0x02B0 */ union { Vec3f unk_2B0; Vec3f rightWingRot; };
    /* 0x02BC */ union { Vec3f unk_2BC; Vec3f headRot; };
    /* 0x02C8 */ union { Vec3f unk_2C8; Vec3f posRotStep; };
    /* 0x02D4 */ union { f32 unk_2D4; f32 focusYOffset; };
    /* 0x02D8 */ union { f32 unk_2D8; f32 removeStateYaw; };
    /* 0x02DC */ union { Vec3f unk_2DC; Vec3f initPos; };
    /* 0x02E8 */ union { Vec3f unk_2E8; Vec3f targetPos; };
    /* 0x02F4 */ union { f32 unk_2F4; f32 scale; };
    /* 0x02F8 */ union { u8 unk_2F8; u8 unkAlleyHitByte; };
    /* 0x02FC */ ColliderCylinder collider;
    /* 0x0348 */ union { EnSyatekiNiw_1 unk_348[5]; EnSyatekiNiw_1 effects[5]; };
} EnSyatekiNiw; // size = 0x0460

#endif
