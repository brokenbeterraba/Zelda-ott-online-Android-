# Alterações desta revisão (ligação do cliente Zelda Online ao motor)

## O que foi feito
1. **Hooks que faltavam no motor** (o cliente os usava e daria erro de compilação):
   - `OnDrawOverlay` — executado em `Play_Draw` (z_play.c) logo após `Play_DrawOverlayElements`.
   - `ShouldActorDelayInit` — consultado em `Actor_Init` e em `Actor_UpdateAll` (z_actor.c); se algum hook pedir,
     o `init` do ator é adiado para o próximo frame (usado pelos controladores de chefes/inimigos).
   Arquivos: `GameInteractor_HookTable.h`, `GameInteractor_Hooks.h/.cpp`, `src/code/z_actor.c`, `src/code/z_play.c`.
2. **`ZeldaOnlineClient::Instance` nunca era definido** (erro de link). Agora é definido em `ZeldaOnlineClient.cpp`
   e atribuído no construtor.
3. **`mZeldaOnlineRoomWindow` era só `extern`** e nunca criado (o menu abriria um ponteiro nulo). Agora a janela e o
   cliente são criados em `SohGui::SetupGuiElements` e liberados em `Destroy`; `ZeldaOnlineRoomWindow::Instance` é atribuído.
4. `ZeldaOnlineMenu.cpp`: `Enable()` do cliente roda uma vez, na primeira abertura da janela (precisa do ImGui pronto).

## NÃO validado (nada disto foi compilado nem testado)
- Build Android arm64 (sem SDK/NDK no ambiente de revisão) — rode o workflow **Android APK build**.
- Os 13 headers divergentes do roadmap e qualquer outro erro de API que só o compilador mostra.
- Sincronização real de chefes, NPCs e itens entre dois aparelhos.
- Rodar o workflow e mandar o primeiro erro real do relatório é o próximo passo.

## Rodada 2 (primeiro erro real do CI)
- `PlayerPuppetController.cpp` e `HorsePuppet.cpp` falhavam com "templates must have C++ linkage" / "conflicting types
  for ResourceLoad": o `bridge.h` do libultraship (C++) era incluído pela primeira vez DENTRO de `extern "C" {}`.
  Correção: `#include <libultraship/libultraship.h>` no topo dos dois arquivos (mesmo padrão do Anchor).
- Esse foi o único arquivo-problema (2 FAILED) no log; como o ninja para ao falhar, o link e outros erros ainda podem aparecer.
