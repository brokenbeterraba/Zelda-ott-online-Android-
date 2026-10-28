package com.zeldaonline.host;

final class ServerNative {
    static {
        System.loadLibrary("ootserver_jni");
    }

    private ServerNative() {}

    static native boolean start(int port, String configPath);
    static native void stop();
    static native int status();
    static native int lastError();
}
