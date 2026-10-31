# Shipwright Online Android — app único

Um só APK: jogo (Shipwright 9.2.3 Android) + cliente multiplayer + servidor embutido (libootserver.so).
No menu do jogo: aba "Entrar" (IP + porta) e aba "Criar servidor" (sobe o servidor e conecta você automaticamente).

## Como aplicar
1. Apague o conteúdo do repositório (exceto .git) e envie estes arquivos (ou extraia por cima).
2. As pastas libultraship/, ZAPDTR/ e OTRExporter/ ficam vazias de propósito: o workflow baixa os commits fixados.
3. Actions > "Android APK build" > Run workflow. O APK sai no artefato `shipwright-online-android-apk`.
4. Se falhar, o primeiro erro é publicado na issue "Relatório de build CI".

## Estado honesto
Verificado só por checagem de sintaxe. Ainda não foi compilado com o NDK nem testado em aparelho.
Faltam ligar o cliente ao motor (OTRGlobals/SohGui/ganchos do GameInteractor) — ver ROADMAP abaixo.

## ROADMAP
3B-2 funções do motor (Actor_SpawnDirect etc.) · 3B-3 ganchos GameInteractor · 3B-4 ligação ao motor ·
3B-5 13 headers divergentes · build real · teste com 2 aparelhos · depois migrar para 9.3.1.
