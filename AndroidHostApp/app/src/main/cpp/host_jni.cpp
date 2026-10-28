#include <jni.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "OOTServer.hpp"

namespace {
std::mutex g_mutex;
std::unique_ptr<ZeldaOnline::OOTServer> g_server;
std::thread g_thread;
std::atomic<int> g_status{0}; // 0 stopped, 1 starting/running, 2 error
std::atomic<int> g_last_error{0}; // 1 socket creation, 2 port bind/listen
std::atomic<int> g_last_result{0};

std::string FromJava(JNIEnv* env, jstring value) {
    if (value == nullptr) return {};
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (chars == nullptr) return {};
    std::string result(chars);
    env->ReleaseStringUTFChars(value, chars);
    return result;
}
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_zeldaonline_host_ServerNative_start(JNIEnv* env, jclass, jint port, jstring configPath) {
    if (port < 1 || port > 65535) return JNI_FALSE;
    const std::string config = FromJava(env, configPath);
    if (config.empty()) return JNI_FALSE;

    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_status.load(std::memory_order_acquire) == 1) return JNI_FALSE;
    if (g_thread.joinable()) g_thread.join();
    g_server.reset();

    g_server = std::make_unique<ZeldaOnline::OOTServer>(static_cast<int>(port), config);
    ZeldaOnline::OOTServer* server = g_server.get();
    g_last_error.store(0, std::memory_order_release);
    g_last_result.store(-1, std::memory_order_release);
    g_status.store(1, std::memory_order_release);

    try {
        g_thread = std::thread([server]() {
            const int result = server->run();
            g_last_error.store(server->LastError(), std::memory_order_release);
            g_last_result.store(result, std::memory_order_release);
            g_status.store(result == 0 ? 0 : 2, std::memory_order_release);
        });
    } catch (...) {
        g_status.store(2, std::memory_order_release);
        g_last_error.store(3, std::memory_order_release);
        g_last_result.store(-1, std::memory_order_release);
        g_server.reset();
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_zeldaonline_host_ServerNative_stop(JNIEnv*, jclass) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_server) g_server->Stop();
    if (g_thread.joinable()) g_thread.join();
    g_server.reset();
    g_status.store(0, std::memory_order_release);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_zeldaonline_host_ServerNative_status(JNIEnv*, jclass) {
    return g_status.load(std::memory_order_acquire);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_zeldaonline_host_ServerNative_lastError(JNIEnv*, jclass) {
    return g_last_error.load(std::memory_order_acquire);
}
