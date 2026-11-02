#ifndef SPOT08BAKUDANKABECONTROLLERH
#define SPOT08BAKUDANKABECONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Spot08_Bakudankabe/z_bg_spot08_bakudankabe.h"

void func_808B0324(BgSpot08Bakudankabe* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class Spot08BakudankabeController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgSpot08Bakudankabe* Typed() const {
        return reinterpret_cast<BgSpot08Bakudankabe*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        BgSpot08Bakudankabe* self = Typed();
        return (self->collider.base.acFlags & AC_HIT) != 0;
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        BgSpot08Bakudankabe* self = Typed();
        func_808B0324(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        BgSpot08Bakudankabe* self = Typed();

        if (BreakWouldFire(play)) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
            return;
        }

        self->collider.base.acFlags &= ~AC_HIT;
        if (self->dyna.actor.xzDistToPlayer < 800.0f) {
            RegisterColliderBase(play, &self->collider.base, COLL_AC);
        }
    }
};

}

#endif
