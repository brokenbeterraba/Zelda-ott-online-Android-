#ifndef SPOT17BAKUDANKABECONTROLLERH
#define SPOT17BAKUDANKABECONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Spot17_Bakudankabe/z_bg_spot17_bakudankabe.h"

void func_808B6BC0(BgSpot17Bakudankabe* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class Spot17BakudankabeController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgSpot17Bakudankabe* Typed() const {
        return reinterpret_cast<BgSpot17Bakudankabe*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        BgSpot17Bakudankabe* self = Typed();
        return (self->dyna.actor.xzDistToPlayer < 650.0f && func_80033684(play, &self->dyna.actor) != nullptr);
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        BgSpot17Bakudankabe* self = Typed();
        func_808B6BC0(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        BgSpot17Bakudankabe* self = Typed();

        if (BreakWouldFire(play)) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
            return;
        }

    }
};

}

#endif
