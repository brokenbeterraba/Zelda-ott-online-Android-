#include "ZeldaOnlineHost.hpp"

#include <algorithm>
#include <cstring>

#if defined(__ANDROID__)
#include <ship/Context.h>
#include "ootserver_api.h" // server/android (libootserver.so)
#endif

#if defined(__ANDROID__) || defined(__linux__)
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netinet/in.h>
#endif

namespace ZeldaOnline::Host {

#if defined(__ANDROID__)

bool Available() {
    return true;
}

StartResult Start(int port) {
    const std::string config = Ship::Context::GetPathRelativeToAppDirectory("zeldaonline-server.json");
    switch (ZoServer_Start(port, config.c_str())) {
        case ZO_SERVER_OK:
            return StartOk;
        case ZO_SERVER_BAD_PORT:
            return BadPort;
        case ZO_SERVER_ALREADY_RUNNING:
            return AlreadyRunning;
        case ZO_SERVER_CONFIG_WRITE_FAILED:
            return ConfigWriteFailed;
        default:
            return ThreadFailed;
    }
}

void Stop() {
    ZoServer_Stop();
}

Status GetStatus() {
    switch (ZoServer_Status()) {
        case ZO_SERVER_RUNNING:
            return Running;
        case ZO_SERVER_ERROR:
            return Error;
        case ZO_SERVER_STARTING:
            return Starting;
        default:
            return Stopped;
    }
}

int LastError() {
    return ZoServer_LastError();
}

int Port() {
    return ZoServer_Port();
}

#else // sem servidor embutido

bool Available() {
    return false;
}
StartResult Start(int) {
    return NotAvailable;
}
void Stop() {}
Status GetStatus() {
    return Stopped;
}
int LastError() {
    return 0;
}
int Port() {
    return 0;
}

#endif

std::vector<LocalAddress> LocalAddresses() {
    std::vector<LocalAddress> result;
#if defined(__ANDROID__) || defined(__linux__)
    ifaddrs* list = nullptr;
    if (getifaddrs(&list) != 0 || list == nullptr)
        return result;

    for (ifaddrs* it = list; it != nullptr; it = it->ifa_next) {
        if (it->ifa_addr == nullptr || it->ifa_addr->sa_family != AF_INET)
            continue;
        if ((it->ifa_flags & IFF_UP) == 0 || (it->ifa_flags & IFF_LOOPBACK) != 0)
            continue;

        // Dados móveis (rmnet/ccmni) ficam atrás de CGNAT: amigos não conseguem entrar por eles.
        const char* name = it->ifa_name ? it->ifa_name : "";
        if (std::strncmp(name, "rmnet", 5) == 0 || std::strncmp(name, "ccmni", 5) == 0 ||
            std::strncmp(name, "dummy", 5) == 0 || std::strncmp(name, "v4-", 3) == 0)
            continue;

        char text[INET_ADDRSTRLEN] = {};
        const auto* addr = reinterpret_cast<const sockaddr_in*>(it->ifa_addr);
        if (inet_ntop(AF_INET, &addr->sin_addr, text, sizeof(text)) == nullptr)
            continue;
        if (std::strncmp(text, "169.254.", 8) == 0) // link-local, sem rede de verdade
            continue;

        result.push_back({ name, text });
    }
    freeifaddrs(list);
#endif
    return result;
}

} // namespace ZeldaOnline::Host
