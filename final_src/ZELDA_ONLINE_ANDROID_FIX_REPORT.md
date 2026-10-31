# Shipwright Android + Zelda Online — pacote de correção

## O que foi alterado
- Removidas todas as referências diretas a `Actor::zoController` da integração Zelda Online.
- Adicionado um registro externo `Actor* -> AbstractActorController*` em `AbstractActorController.hpp`, sem alterar o tamanho/layout de `Actor`.
- O registro é preenchido na criação do controlador e limpo na destruição/desanexação.
- `DispatchInit` e `DispatchUpdate` verificam se existe controlador antes de chamar seus métodos.
- Mantido o menu de rede Zelda Online que já estava integrado à árvore Android.

## Estado de validação
- **Servidor:** compilado com sucesso neste ambiente Linux via CMake. Foram emitidos avisos de compilação, mas nenhum erro fatal.
- **Cliente Android:** não foi possível validar uma compilação completa neste ambiente porque o ZIP fornecido não contém os submódulos externos necessários (por exemplo, `libultraship`). Não afirmo que o APK foi compilado nem que a sincronização em jogo foi testada.
- Esta alteração corrige o erro conhecido (`Actor` não possui membro `zoController`), mas não garante que não existam incompatibilidades adicionais com a revisão Android.

## Cliente Android
O menu de conexão já existente fica em `Network > Zelda Online`. A janela possui endereço/host, porta, apelido e o botão `Connect`; depois de conectar, `Create Party` cria uma sala/party no servidor.

## Servidor
O servidor é um processo separado nesta versão. Não está embutido como serviço de hospedagem dentro do APK Android.

Requisitos: CMake 3.16+ e compilador C++20.

Linux:
```bash
cmake -S server -B build-server -DCMAKE_BUILD_TYPE=Release
cmake --build build-server -j2
./build-server/OOTServer --port 21050
```

O servidor precisa ficar acessível na rede. Para hospedar fora da rede local, configure o encaminhamento da porta TCP 21050 no roteador/firewall. `127.0.0.1` no Android aponta para o próprio celular, não para outro dispositivo.

## Compilação do APK
Use o ambiente de build do próprio repositório e inicialize os submódulos Git antes de compilar:
```bash
git submodule update --init --recursive
```
A compilação completa do cliente Android ainda precisa ser executada no ambiente com SDK/NDK e submódulos corretos.
