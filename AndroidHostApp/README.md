# Zelda Online Host para Android

Segundo aplicativo Android, compilado junto com o jogo a partir do mesmo repositório, que inicia o servidor C++ do **mesmo código-fonte Shipwright-Online** em uma thread nativa dentro do próprio processo Android. Não tenta executar o binário Linux. O servidor é compilado pelo NDK como `libootserver_jni.so` e controlado por uma Activity e um serviço em primeiro plano.

## Compatibilidade

- Android 7.0 (API 24) ou superior.
- ABI configurada para `arm64-v8a`, apropriada para a maioria dos celulares Android atuais, incluindo o Moto G60.
- Porta TCP padrão: `21050`, a mesma porta padrão do servidor incluído em `server/`.

## Compilar o APK

É necessário instalar JDK 17 ou superior, Android SDK com plataforma 35, Android NDK e CMake 3.22.1 pelo SDK Manager. Para compilar apenas o host, na pasta `AndroidHostApp`:

```bash
./gradlew assembleDebug
```

O APK deve ser gerado em:

```text
app/build/outputs/apk/debug/app-debug.apk
```

A compilação precisa acontecer com a pasta `server/` original ao lado de `AndroidHostApp/`, como está neste repositório. O CMake importa os arquivos `.cpp` do servidor sem duplicar a implementação nem seu protocolo.

## Como usar

1. Instale e abra `Zelda Online Host` no aparelho que hospedará a partida.
2. Confira a porta TCP (padrão `21050`) e, opcionalmente, as configurações de relógio/reset das masmorras.
3. Toque em **Iniciar host**. O serviço em primeiro plano e a notificação mantêm visível que o servidor está ativo; toque em **Parar servidor** para encerrá-lo.
4. Na versão Android/PC do Shipwright-Online, abra `Network → Zelda Online` e conecte ao IP mostrado pelo host, com a mesma porta.
5. No mesmo aparelho do host, tente `127.0.0.1` como endereço do servidor. Em outro dispositivo na mesma rede, use o IP LAN mostrado no aplicativo.

Para aceitar conexões de fora da rede local, normalmente é preciso encaminhar a porta TCP no roteador ou usar uma VPN/túnel entre os jogadores. O IP LAN não é um endereço público.

## Limites desta entrega

- Este é o **código-fonte** de um app host; não é um APK já compilado.
- O ambiente onde o pacote foi preparado não possui Android SDK/NDK configurados; portanto, não foi possível validar a compilação do APK aqui.
- A compilação do servidor para Linux e o protocolo do servidor pertencem ao código existente, mas ainda não foi testado neste pacote um cliente Android conectando a uma sessão real hospedada no celular.
- As opções de inimigos/chefes e outros estados continuam limitadas pelo estado de implementação da sincronização do Shipwright-Online; iniciar o host não adiciona sincronização de gameplay que não exista no protocolo atual.
- A porta está sem autenticação/criptografia no protocolo existente. Use sessões confiáveis; não exponha o serviço publicamente sem entender esse risco.

Também há `build-apk.sh` para compilar localmente e o workflow `.github/workflows/build-android-host.yml` para gerar automaticamente o APK como artefato do GitHub Actions quando o código for enviado a um repositório GitHub. Esse workflow ainda não foi executado nesta entrega; ele serve como configuração de compilação, não como prova de APK compilado.
