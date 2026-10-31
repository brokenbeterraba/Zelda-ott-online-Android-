#ifndef Z_EN_PU_BOX_H
#define Z_EN_PU_BOX_H

#include <libultraship/libultra.h>
#include "global.h"

struct EnPubox;

typedef struct EnPubox {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ union { u32 unk_164; u32 unused_164; };
} EnPubox; // size = 0x0168

#endif
