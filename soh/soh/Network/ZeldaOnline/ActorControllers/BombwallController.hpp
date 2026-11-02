#ifndef BOMBWALLCONTROLLERH
#define BOMBWALLCONTROLLERH

#include "../AbstractActorController.hpp"

extern "C" {
#include "src/overlays/actors/ovl_Bg_Bombwall/z_bg_bombwall.h"

void func_8086ED70(BgBombwall* self, PlayState* play); // esperando explosao
void func_8086EE40(BgBombwall* self, PlayState* play); // quebrando (delay)
void func_8086EDFC(BgBombwall* self, PlayState* play); // inicia a quebra (poeira)
void func_8086EE94(BgBombwall* self, PlayState* play); // parede quebrada (remove colisao)
}

namespace ZeldaOnline {

// Parede de bomba que nao some (so troca o modelo). O estado vai por propriedade: 0 = inteira, 1 = quebrando,
// 2 = quebrada. Quem acerta vira lider e roda o Update real.
class BombwallController : public AbstractActorController {
  public:
    using AbstractActorController::AbstractActorController;

    BgBombwall* Typed() const {
        return reinterpret_cast<BgBombwall*>(m_actor);
    }

  protected:
    u8 State() const {
        BgBombwall* w = Typed();
        if (w->actionFunc == func_8086ED70)
            return 0;
        if (w->actionFunc == func_8086EE40)
            return 1;
        if (w->actionFunc == nullptr)
            return 2;
        return 0xFF; // ainda inicializando
    }

    enum {
        PROP_STATE = PROP_CUSTOM_START,
    };

    void BuildCustomProperties(ByteStream& out) override {
        PackProperty(PROP_STATE, PackedUInt1(State()), out);
    }

    bool ApplyCustomProperty(unsigned int index, ByteStream& data, unsigned int propLen) override {
        BgBombwall* w = Typed();
        switch (index) {
            case PROP_STATE: {
                u8 wanted = (u8)(data.Read<PackedUInt1>().value());
                if (gPlayState == nullptr || wanted == 0 || wanted == 0xFF)
                    break;
                u8 cur = State();
                if (cur == 0)
                    func_8086EDFC(w, gPlayState); // poeira + vai para "quebrando"
                if (wanted == 2 && State() == 1)
                    func_8086EE94(w, gPlayState); // troca modelo e remove colisao
                break;
            }
            default:
                return false;
        }
        return true;
    }

    void UpdatePuppet(PlayState* play) override {
        BgBombwall* w = Typed();

        if (State() != 0)
            return;

        if (w->collider.base.acFlags & AC_HIT) {
            ClaimLeadership(CLAIM_REASON_NOW);
            UpdateLeader(play);
            return;
        }

        if (w->dyna.actor.xzDistToPlayer < 600.0f)
            RegisterColliderBase(play, &w->collider.base, COLL_AC);
    }
};

}

#endif
