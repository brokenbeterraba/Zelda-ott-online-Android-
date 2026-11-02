#ifndef HIDANKOWARERUKABECONTROLLERH
#define HIDANKOWARERUKABECONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Hidan_Kowarerukabe/z_bg_hidan_kowarerukabe.h"

void BgHidanKowarerukabe_Break(BgHidanKowarerukabe* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class HidanKowarerukabeController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgHidanKowarerukabe* Typed() const {
        return reinterpret_cast<BgHidanKowarerukabe*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        BgHidanKowarerukabe* self = Typed();
        return Actor_GetCollidedExplosive(play, &self->collider.base) != nullptr;
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        BgHidanKowarerukabe* self = Typed();
        BgHidanKowarerukabe_Break(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        BgHidanKowarerukabe* self = Typed();

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
