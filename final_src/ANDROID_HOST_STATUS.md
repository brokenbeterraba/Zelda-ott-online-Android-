# Android host integration status

## Implemented in source

- Separate `AndroidHostApp` Gradle application project with a native C++ server library.
- Uses the existing `server/src` implementation and existing Zelda Online TCP protocol/version (`BETA000002`, default port TCP 21050), not a new/incompatible protocol.
- JNI entry points to start, stop and query server status.
- Thread-safe stop/request state on the server run loop; configuration path can be supplied by the Android app instead of relying on the process working directory.
- Foreground Android service, persistent status notification, CPU partial wake lock while hosting, and UI for port, LAN IP/copy, day/night speeds and dungeon reset policy/timer.
- Server configuration retains the original configured skin list from `server/config.json`.

## Validations completed

- `server/src/OOTServer.cpp` passed a C++20 syntax-only compilation after the change.
- `AndroidHostApp/app/src/main/cpp/host_jni.cpp` passed a C++20 syntax-only compilation against the installed JDK JNI headers.
- Android manifest XML parsed successfully.
- JNI method names/signatures were checked against the Java declarations.
- CMake's relative path to the sibling `server/src` tree was checked.

## Not yet validated

- Full Android Gradle/NDK build and APK installation: Android SDK/NDK are unavailable in this build environment.
- Actual Android device runtime, background behavior across vendor battery policies, LAN connectivity, and a real client joining the host.
- Public internet accessibility without router port forwarding or a tunnel/VPN.

This integration starts the server inside an Android app process. It does not add enemy/boss synchronization that is missing from the current multiplayer protocol.
