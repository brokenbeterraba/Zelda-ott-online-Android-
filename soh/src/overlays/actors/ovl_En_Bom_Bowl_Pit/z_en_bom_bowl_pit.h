#ifndef Z_EN_BOM_BOWL_PIT_H
#define Z_EN_BOM_BOWL_PIT_H

#include <libultraship/libultra.h>
#include "global.h"
#include "overlays/actors/ovl_En_Ex_Item/z_en_ex_item.h"

struct EnBomBowlPit;

typedef void (*EnBomBowlPitActionFunc)(struct EnBomBowlPit*, PlayState*);

typedef struct EnBomBowlPit {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ EnBomBowlPitActionFunc actionFunc;
    /* 0x0150 */ u16 unk_150;
    /* 0x0152 */ s16 camId;
    /* 0x0154 */ s16 unk_154;
    /* 0x0156 */ s16 exItemDone; // Set by EnExItem?
    /* 0x0158 */ s16 timer;
    /* 0x015A */ s16 prizeIndex;
    /* 0x015C */ s16 start;
    /* 0x0160 */ s32 getItemId;
    /* 0x0164 */ u8 status;
    /* 0x0168 */ union { Vec3f unk_168; Vec3f viewAt; }; // set and not used?
    /* 0x0174 */ union { Vec3f unk_174; Vec3f viewEye; }; // set and not used?
    /* 0x0180 */ union { Vec3f unk_180; Vec3f subCamAt; }; // camera at (start)
    /* 0x018C */ union { Vec3f unk_18C; Vec3f subCamEye; }; // camera eye (start)
    /* 0x0198 */ union { Vec3f unk_198; Vec3f subCamEyeNext; }; // camera eye (end)
    /* 0x01A4 */ union { Vec3f unk_1A4; Vec3f subCamEyeMaxVelFrac; }; // camera eye (scales)
    /* 0x01B0 */ union { Vec3f unk_1B0; Vec3f subCamEyeVel; }; // camera eye (maxsteps)
    /* 0x01BC */ union { Vec3f unk_1BC; Vec3f subCamAtNext; }; // camera at (end)
    /* 0x01C8 */ union { Vec3f unk_1C8; Vec3f subCamAtMaxVelFrac; }; // camera at (scales)
    /* 0x01D4 */ union { Vec3f unk_1D4; Vec3f subCamAtVel; }; // camera eye (maxsteps)
    /* 0x01E0 */ EnExItem* exItem;
    /* 0x01E4 */ char unk_1E4[0x3520];
} EnBomBowlPit; // size = 0x3704

#endif
