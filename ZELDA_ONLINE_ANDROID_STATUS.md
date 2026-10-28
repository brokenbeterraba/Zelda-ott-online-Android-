# Shipwright Android + Zelda Online — pacote de código-fonte

Este pacote reúne a árvore Android com a integração Zelda Online que estava disponível no projeto e a correção do acesso a controladores de atores.

## Correção incluída
A integração não adiciona `zoController` à estrutura `Actor` (isso alteraria o layout ABI do engine). Em vez disso, usa um registro externo `Actor* -> AbstractActorController*` e os acessos da integração consultam esse registro.

## O que está presente
- Cliente Zelda Online integrado ao menu do Shipwright.
- Janela com endereço/host, porta, apelido e conexão.
- Sistema de salas/parties do Zelda Online.
- Código-fonte do servidor dedicado em `server/`.
- Registro externo de controladores para não ampliar `Actor`.

## Limites importantes — leia antes de usar
Este pacote **não é um APK Android pronto** e não posso declarar o cliente Android compilado/testado em jogo. A árvore fornecida não contém os submódulos Git externos completos (`libultraship`, `ZAPDTR`, `OTRExporter`), necessários para a compilação completa do cliente.
O servidor dedicado foi compilado com sucesso para Linux neste ambiente; isso não valida a compilação Android nem uma sessão multiplayer real.
O menu atual conecta ao servidor dedicado e cria parties; ele **não inicia o servidor dentro do próprio APK**. Para hospedar, execute o servidor separadamente em um computador ou dispositivo que consiga compilá-lo. A hospedagem embutida no APK ainda não está implementada.

## Compilar o servidor em Linux
```bash
cmake -S server -B build-server -DCMAKE_BUILD_TYPE=Release
cmake --build build-server -j2
./build-server/OOTServer --port 21050
```

## Compilar o servidor no Termux (Android ARM64)
Instale as ferramentas no Termux:
```bash
pkg update
pkg install clang cmake make
```
Entre na pasta `server` do projeto e compile:
```bash
cmake -S server -B build-server -DCMAKE_BUILD_TYPE=Release
cmake --build build-server -j2
./build-server/OOTServer --port 21050
```
Se o programa ignorar o argumento da porta, confira `server/src/main.cpp` e a configuração do servidor. Para acesso de fora da rede local, é necessário encaminhar a porta TCP no roteador/firewall; para LAN, use o IP local do dispositivo que está executando o servidor. `127.0.0.1` significa o próprio aparelho.

## Preparar o cliente Android
1. Inicialize os submódulos no clone Git original:
   `git submodule update --init --recursive`
2. Configure Android SDK/NDK compatíveis com a versão-base.
3. Abra o projeto Android no ambiente de build indicado pelo repositório e compile.
4. Teste primeiro dois clientes na mesma rede e com o mesmo endereço/porta do servidor.

## Escopo de sincronização
A integração Zelda Online possui lógica de rede e sincronização específica, mas isso não garante que cada inimigo, chefe, cutscene ou estado de mundo esteja perfeitamente sincronizado. É necessário testar dois clientes conectados em uma sessão real.

## Referências dos projetos
- HarbourMasters/Shipwright: https://github.com/HarbourMasters/Shipwright
- Shipwright-Android: https://github.com/linkzenic/Shipwright-Android
- Shipwright-Online: https://github.com/fReEsPiRiT-Spiritsoft/Shipwright-Online
