# Correções de compatibilidade Zelda Online / Shipwright Android

Esta revisão parte do ZIP `codigo-fonte-final.zip` recebido em 31/10/2026.

## Correções aplicadas

- Implementadas as APIs `Actor_SpawnDirect` e `Actor_SpawnAsChildDirect` usadas pelo cliente Zelda Online. A inicialização usa o transform de home e aplica depois o transform de mundo recebido.
- Expostos `FishingFishInit` e `sFishInits` no header do ator de pesca, removendo a definição `static` que impedia o acesso pelo controlador de pesca.
- Movido `DoorKillerBehaviour` para o header público para disponibilizar `DOOR_KILLER_DOOR` ao controlador multiplayer.
- Implementadas as APIs de nametag `NameTag_RemoveAllByActorTag` e `NameTag_ChangeActorTextColour` usadas pelo cliente e pelos controladores.
- Para esta revisão do motor, `EnHy_Fidget` não existe como símbolo exportado; o controlador usa uma adaptação inline para `EnHy_DoNothing`, evitando referência indefinida. O comportamento de fidget não é reproduzido nessa adaptação.

## Limites de validação

- Não foi possível executar a compilação Android completa neste ambiente: Android SDK/NDK e dependências nativas do projeto não estão instalados aqui.
- As correções de integração precisam ser confirmadas no workflow Android arm64. Uma compilação bem-sucedida ainda não substitui teste real de duas instâncias conectadas, spawn de atores, pesca, nametags e hospedagem.
- O host continua integrado ao mesmo APK por meio da biblioteca nativa `ootserver`; não há segundo APK nesta revisão.
