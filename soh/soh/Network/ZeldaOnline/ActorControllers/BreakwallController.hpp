#ifndef BREAKWALLCONTROLLERH
#define BREAKWALLCONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Breakwall/z_bg_breakwall.h"

void BgBreakwall_Wait(BgBreakwall* self, PlayState* play);

Actor* BgBreakwall_SpawnFragments(PlayState* play, BgBreakwall* self, Vec3f* pos, f32 velocity, f32 scaleY, f32 scaleX,
                                  s32 count, f32 accel);
}

namespace ZeldaOnline {

// Parede/chao de bomba (Dodongo's Cavern e outros). A deteccao da explosao acontece no Draw (quad collider), entao
// qualquer jogador que veja a explosao vira lider; os outros recebem a morte e rodam so os fragmentos.
class BreakwallController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgBreakwall* Typed() const {
        return reinterpret_cast<BgBreakwall*>(m_actor);
    }

  protected:
    void OnServerDestroy() override {
        BgBreakwall* w = Typed();
        if (gPlayState == nullptr || w == nullptr)
            return;

        Vec3f effectPos;
        effectPos.x = effectPos.y = effectPos.z = 0.0f;
        if (w->dyna.actor.world.rot.x == 0) {
            effectPos.y = 55.0f;
        } else {
            effectPos.z = 25.0f;
            effectPos.y = -10.0f;
        }

        BgBreakwall_SpawnFragments(gPlayState, w, &effectPos, 0.0f, 6.4f, 5.0f, 1, 2.0f);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &w->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        BgBreakwall* w = Typed();

        if (w->actionFunc == BgBreakwall_Wait && (w->collider.base.acFlags & AC_HIT)) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
        }
    }
};

}

#endif
