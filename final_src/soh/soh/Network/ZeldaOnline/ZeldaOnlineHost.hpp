#pragma once
// Servidor ZeldaOnline embutido no app (Android). Em outras plataformas Available() é false
// e use o servidor dedicado de server/.
#include <string>
#include <vector>

namespace ZeldaOnline::Host {

struct LocalAddress {
    std::string iface; // ex.: wlan0
    std::string ip;    // ex.: 192.168.0.12
};

enum Status { Stopped = 0, Running = 1, Error = 2, Starting = 3 };

enum StartResult { StartOk = 0, BadPort, AlreadyRunning, ConfigWriteFailed, ThreadFailed, NotAvailable };

bool Available();
StartResult Start(int port);
void Stop();
Status GetStatus();
int LastError(); // 1 = socket, 2 = porta ocupada/sem permissão, 3 = thread
int Port();

// Endereços IPv4 deste aparelho que outros aparelhos da mesma rede conseguem usar.
std::vector<LocalAddress> LocalAddresses();

} // namespace ZeldaOnline::Host
