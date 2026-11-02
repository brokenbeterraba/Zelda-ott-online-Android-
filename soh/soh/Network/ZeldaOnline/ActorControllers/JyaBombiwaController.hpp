#ifndef JYABOMBIWACONTROLLERH
#define JYABOMBIWACONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Jya_Bombiwa/z_bg_jya_bombiwa.h"

void BgJyaBombiwa_Break(BgJyaBombiwa* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class JyaBombiwaController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgJyaBombiwa* Typed() const {
        return reinterpret_cast<BgJyaBombiwa*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        BgJyaBombiwa* self = Typed();
        return (self->collider.base.acFlags & AC_HIT) != 0;
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        BgJyaBombiwa* self = Typed();
        BgJyaBombiwa_Break(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        BgJyaBombiwa* self = Typed();

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
