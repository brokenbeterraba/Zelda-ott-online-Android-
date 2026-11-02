#ifndef SPOT11BAKUDANKABECONTROLLERH
#define SPOT11BAKUDANKABECONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Spot11_Bakudankabe/z_bg_spot11_bakudankabe.h"

void func_808B2218(BgSpot11Bakudankabe* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class Spot11BakudankabeController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgSpot11Bakudankabe* Typed() const {
        return reinterpret_cast<BgSpot11Bakudankabe*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        BgSpot11Bakudankabe* self = Typed();
        return (self->collider.base.acFlags & AC_HIT) != 0;
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        BgSpot11Bakudankabe* self = Typed();
        func_808B2218(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        BgSpot11Bakudankabe* self = Typed();

        if (BreakWouldFire(play)) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
            return;
        }

        self->collider.base.acFlags &= ~AC_HIT;
        if (self->dyna.actor.xzDistToPlayer < 100000.0f) {
            RegisterColliderBase(play, &self->collider.base, COLL_AC);
        }
    }
};

}

#endif
