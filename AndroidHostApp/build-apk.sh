#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
if [[ -z "${ANDROID_HOME:-}" && -z "${ANDROID_SDK_ROOT:-}" ]]; then
  echo "Android SDK is not configured. Set ANDROID_HOME or ANDROID_SDK_ROOT first." >&2
  exit 2
fi
./gradlew --no-daemon assembleDebug
printf '\nAPK generated at: %s\n' "$PWD/app/build/outputs/apk/debug/app-debug.apk"
