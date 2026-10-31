# Ship of Harkinian Android

Android ports of Ship of Harkinian, based on the HarbourMasters project.

Original repository: https://github.com/HarbourMasters/Shipwright

Current Android release: **v9.2.3-android.13**

## Which port should I use?

**For most people, regular Ship of Harkinian is the recommended choice.** It
most closely follows the standard Harbour Masters experience. Choose one of the
other ports only when you specifically want that fork's changes.

| Port | What it is for | Recommendation |
| --- | --- | --- |
| **Ship of Harkinian** | The regular Android port, closest to upstream | **Recommended for most players** |
| **Fusion Fork** | The combined experimental fork | Use when you specifically want the Fusion feature set |
| **SOHCS** | Roborich's cel-shaded fork | Use when you specifically want the cel-shaded presentation |
| **SOHNEI** | Skijer's Not Enough Items fork | Use when you specifically want the added NEI content |

The ports use separate app IDs and can be installed together.

Supported: Android 7+ with OpenGL ES 3.0+

Tested on: Android 15

Official website: https://www.shipofharkinian.com/

Official Discord: https://discord.com/invite/shipofharkinian

## Project Attribution

[Ship of Harkinian](https://github.com/HarbourMasters/Shipwright) is developed by Harbour Masters and its contributors.

## Installation

1. Verify that you have a supported, legally obtained Ocarina of Time ROM. You can use the compatibility checker at https://ship.equipment/ or compare your ROM's `sha1` hash against [docs/supportedHashes.json](docs/supportedHashes.json).
2. Install regular SOH (recommended), Fusion, SOHCS, or SOHNEI from the releases page: https://github.com/linkzenic/Shipwright-Android/releases
3. Open the app once so it can create the data folder and copy bundled support files.
4. When prompted, allow setup and select your ROM so the app can generate the required `.otr` or `.o2r` game data.
5. If you have a Master Quest ROM, choose to extract another ROM when prompted. Otherwise, continue into the game.
6. Subsequent launches should start directly into the game.

Use the Back, Select, or minus controller button, or the Android back gesture/button, to open the Ship of Harkinian menu. Use touch controls or a controller to navigate menus.

## Data Folder

Standard SOH stores user data in the selected `SOH` data folder. SOHCS uses a separate `SOHCS` data folder so both editions can coexist with independent saves and settings. You can view the current folder and change it from Settings > General.

When SOHCS detects an existing `SOH` folder on first launch, it asks whether to copy compatible archives, saves, settings, and controller configuration. Choose **No** to keep the two editions completely independent. Importing never moves or deletes the original `SOH` files.

Mods and user files should be placed in the relevant folders inside the selected edition's data folder. Mods use `.otr` or `.o2r` files and can be enabled from Settings > Mod Menu.

## FAQ

**Why is it immediately crashing?**

Try deleting and regenerating your extracted `.otr` or `.o2r` game data from your own ROM.

**The game opened once but now shows a black screen.**

Try deleting `imgui.ini` from your `SOH` folder. If it still happens, set MSAA to 1 in Settings > Graphics.

**My controller is not doing anything.**

Open the menu and check Settings > Controls to confirm the controller is detected and mapped. If this happens immediately after first setup, close and reopen the app once.

**Can I hide the on-screen touch controls?**

Yes. Use Settings > Touch Controls > Disable Touch Controls.

**Can I resize the menu?**

Yes. Use Settings > General > Menu Scale.

**How do I add mods?**

Place mod `.otr` or `.o2r` files in the `mods` folder inside the selected edition's data folder, then enable them from Settings > Mod Menu.

##  **Cel-Shaded Fork**

The cel-shaded edition, based on [Roborich's 9.2.3-celshade0.5 fork](https://github.com/roborich/Shipwright/releases/tag/9.2.3-celshade0.5). It uses the /SOHCS data folder.
On first launch, SOHCS asks whether to import existing saves, settings, controller configuration, and compatible archives from /SOH. Choose No to keep both installations completely independent. The original /SOH files are never moved or deleted.

##  **Skijer's Not Enough Items Fork**

This is a beta fork, based on [Skijer's Not Enough Items fork](https://github.com/skijer/Shipwright/releases/tag/Alpha_3). It uses the /SOHNEI data folder.
On first launch, SOHNEI offers to import existing SOH data from /SOH and mm.o2r from /2S2H. It will also import relevant 3DS textures from 2S2H of available. The original files are never moved or deleted.

## Android companion host (Zelda Online)

This source bundle now includes `AndroidHostApp/`, a companion Android app that runs the included `server/` implementation as native C++ inside an Android foreground service. It provides Host Start/Stop, configurable TCP port (default 21050), LAN IP display/copy, day/night time speed and dungeon reset options. Build instructions and networking notes are in `AndroidHostApp/README.md`.

The companion app source has been syntax-checked for its JNI layer and Android manifest, but it has **not** been built into an APK in this environment because Android SDK/NDK are not installed here. The host provides the server process; it does not expand the current gameplay synchronization features of the existing Shipwright-Online protocol.

## Zelda Online Android: dois APKs a partir do mesmo código-fonte

O projeto inclui a pasta `AndroidHostApp/` ao lado de `Android/` e do servidor original `server/`. Não são dois forks: ambos os aplicativos são mantidos e compilados a partir deste mesmo checkout.

- `Android/`: APK do jogo Shipwright-Online.
- `AndroidHostApp/`: APK separado `Zelda Online Host`, que compila o servidor C++ como biblioteca nativa Android por meio do NDK.

Para gerar os dois APKs num único fluxo, inicialize os submódulos e configure o Android SDK/NDK, as dependências CMake/Ninja do projeto e execute na raiz:

```bash
git submodule update --init --recursive
./build-android-apps.sh
```

Os arquivos finais serão colocados em `dist/` como `Shipwright-Online-debug.apk` e `ZeldaOnline-Host-debug.apk`, junto com `SHA256SUMS.txt`. O workflow `.github/workflows/build-both-android-apps.yml` faz essa mesma compilação em CI e publica os dois APKs como um único artefato do GitHub Actions. O workflow só comprova a compilação depois de ser executado com sucesso; até lá, os APKs não devem ser considerados testados.
