#ifndef BOMBIWACONTROLLERH
#define BOMBIWACONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Obj_Bombiwa/z_obj_bombiwa.h"

void ObjBombiwa_Break(ObjBombiwa* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class BombiwaController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    ObjBombiwa* Typed() const {
        return reinterpret_cast<ObjBombiwa*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        ObjBombiwa* self = Typed();
        return (func_80033684(play, &self->actor) != nullptr) ||
               ((self->collider.base.acFlags & AC_HIT) && self->collider.info.acHitInfo != nullptr &&
                (self->collider.info.acHitInfo->toucher.dmgFlags & 0x40000040));
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        ObjBombiwa* self = Typed();
        ObjBombiwa_Break(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        ObjBombiwa* self = Typed();

        if (BreakWouldFire(play)) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
            return;
        }

        self->collider.base.acFlags &= ~AC_HIT;
        if (self->actor.xzDistToPlayer < 800.0f) {
            RegisterColliderBase(play, &self->collider.base, COLL_AC);
        }
    }
};

}

#endif
