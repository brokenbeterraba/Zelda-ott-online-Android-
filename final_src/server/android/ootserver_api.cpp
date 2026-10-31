#include "ootserver_api.h"

#include <atomic>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "OOTServer.hpp"

extern const char* const kZoDefaultConfig; // gerado a partir de server/config.json (default_config.cpp.in)

namespace {
std::mutex g_mutex;
std::unique_ptr<ZeldaOnline::OOTServer> g_server;
std::thread g_thread;
std::atomic<bool> g_finished{true};  // thread terminou (ou nunca iniciou)
std::atomic<int> g_result{0};
std::atomic<int> g_lastError{0};
std::atomic<int> g_port{0};

bool EnsureConfig(const std::string& path) {
    if (path.empty()) return false;
    {
        std::ifstream in(path, std::ios::binary);
        if (in.good()) return true;
    }
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.good()) return false;
    out << kZoDefaultConfig;
    return out.good();
}

void StopLocked() {
    if (g_server) g_server->Stop();
    if (g_thread.joinable()) g_thread.join();
    g_server.reset();
    g_finished.store(true, std::memory_order_release);
    g_result.store(0, std::memory_order_release);
    g_lastError.store(0, std::memory_order_release);
}
} // namespace

extern "C" {

int ZoServer_Start(int port, const char* configPath) {
    if (port < 1 || port > 65535) return ZO_SERVER_BAD_PORT;

    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_thread.joinable() && !g_finished.load(std::memory_order_acquire)) {
        return ZO_SERVER_ALREADY_RUNNING;
    }
    StopLocked(); // limpa uma execução anterior que terminou com erro

    const std::string config = configPath ? configPath : "";
    if (!EnsureConfig(config)) return ZO_SERVER_CONFIG_WRITE_FAILED;

    g_server = std::make_unique<ZeldaOnline::OOTServer>(port, config);
    ZeldaOnline::OOTServer* server = g_server.get();
    g_port.store(port, std::memory_order_release);
    g_finished.store(false, std::memory_order_release);
    try {
        g_thread = std::thread([server]() {
            const int result = server->run();
            g_lastError.store(server->LastError(), std::memory_order_release);
            g_result.store(result, std::memory_order_release);
            g_finished.store(true, std::memory_order_release);
        });
    } catch (...) {
        g_server.reset();
        g_finished.store(true, std::memory_order_release);
        g_result.store(1, std::memory_order_release);
        g_lastError.store(ZO_SERVER_ERR_THREAD, std::memory_order_release);
        return ZO_SERVER_THREAD_FAILED;
    }
    return ZO_SERVER_OK;
}

void ZoServer_Stop(void) {
    std::lock_guard<std::mutex> lock(g_mutex);
    StopLocked();
}

int ZoServer_Status(void) {
    if (g_finished.load(std::memory_order_acquire)) {
        return g_result.load(std::memory_order_acquire) == 0 ? ZO_SERVER_STOPPED : ZO_SERVER_ERROR;
    }
    // O servidor está sendo executado; só vale "rodando" depois que a porta abriu.
    // g_server só muda sob g_mutex, mas Status pode ser chamado de outra thread: protege.
    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_server && g_server->IsRunning()) return ZO_SERVER_RUNNING;
    return ZO_SERVER_STARTING;
}

int ZoServer_LastError(void) { return g_lastError.load(std::memory_order_acquire); }
int ZoServer_Port(void) { return g_port.load(std::memory_order_acquire); }

} // extern "C"
