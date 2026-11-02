#ifndef HAMISHICONTROLLERH
#define HAMISHICONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Obj_Hamishi/z_obj_hamishi.h"

void ObjHamishi_Break(ObjHamishi* self, PlayState* play);
}

namespace ZeldaOnline {

// Objeto que quebra uma unica vez. Quem detecta o golpe vira lider e roda o Update real (quebra, liga a flag
// e faz Actor_Kill); os demais jogadores recebem a morte e rodam apenas a animacao de quebra.
class HamishiController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    ObjHamishi* Typed() const {
        return reinterpret_cast<ObjHamishi*>(m_actor);
    }

  protected:
    bool BreakWouldFire(PlayState* play) const {
        ObjHamishi* self = Typed();
        return (self->collider.base.acFlags & AC_HIT) && self->collider.info.acHitInfo != nullptr &&
               (self->collider.info.acHitInfo->toucher.dmgFlags & 0x40000040);
    }

    enum {
        PROP_HIT_COUNT = PROP_CUSTOM_START,
    };

    void BuildCustomProperties(ByteStream& out) override {
        PackProperty(PROP_HIT_COUNT, PackedInt2(Typed()->hitCount), out);
    }

    bool ApplyCustomProperty(unsigned int index, ByteStream& data, unsigned int propLen) override {
        switch (index) {
            case PROP_HIT_COUNT:
                Typed()->hitCount = (s16)(data.Read<PackedInt2>().value());
                break;
            default:
                return false;
        }
        return true;
    }

    void OnServerDestroy() override {
        if (gPlayState == nullptr || Typed() == nullptr)
            return;
        ObjHamishi* self = Typed();
        ObjHamishi_Break(self, gPlayState);
        SoundSource_PlaySfxAtFixedWorldPos(gPlayState, &self->actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
    }

    void UpdatePuppet(PlayState* play) override {
        ObjHamishi* self = Typed();

        if (BreakWouldFire(play)) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
            return;
        }

        self->collider.base.acFlags &= ~AC_HIT;
        if (self->actor.xzDistToPlayer < 600.0f) {
            RegisterColliderBase(play, &self->collider.base, COLL_AC);
        }
    }
};

}

#endif
