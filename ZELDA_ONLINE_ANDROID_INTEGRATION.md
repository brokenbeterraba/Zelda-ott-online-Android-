# Zelda Online integrado na base Shipwright Android

Esta árvore contém o cliente Zelda Online copiado do projeto ShipwrightOnline `develop` e o menu de rede que registra a seção Zelda Online. O CMake do projeto usa `file(GLOB_RECURSE ...)` para `soh/soh/*.cpp`, então os novos arquivos são descobertos na próxima configuração CMake.

## Importante: estado da integração
Esta é uma integração de código-fonte, não uma compilação validada. O cliente foi originalmente desenvolvido sobre outra revisão do Shipwright e pode exigir ajustes de API ao compilar nesta base Android. Não afirmar multiplayer funcional até compilar e testar dois dispositivos.

## Como usar
1. Compile o APK pelo workflow Android existente.
2. Hospede o servidor da pasta `server/` em Linux/Windows conforme os arquivos CMake/documentação dessa pasta.
3. Abra a porta TCP configurada no cliente (padrão indicado pelo código: 21050) no firewall/roteador.
4. Em cada Android, abra Network > Zelda Online, informe o mesmo endereço DNS/IP público do servidor e a porta.
5. O host cria a party; o servidor envia o evento de entrada ao criador. Os outros jogadores conectam ao mesmo servidor e usam a função de entrar/convite da party.

## Limites
- Não há servidor público embutido. `127.0.0.1` significa o próprio celular, não a internet.
- Estar na mesma party não comprova sincronização completa de todos os atores/estados do jogo.
- Se o workflow falhar, corrigir as incompatibilidades apontadas no primeiro erro real do log antes de publicar APK.
