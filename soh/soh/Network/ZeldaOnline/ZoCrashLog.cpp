// Diagnóstico: grava um backtrace em <pasta do app>/crash_native.log quando o jogo recebe um sinal fatal.
// Só roda no Android. O arquivo fica junto do Ship_of_Harkinian.log.
#ifdef __ANDROID__
#include <cstdint>
#include <dlfcn.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <unwind.h>
#include <string>

namespace {
char sCrashPath[1024] = { 0 };
int sFd = -1;

struct BtState {
    int count;
};

void WriteStr(const char* s) {
    if (sFd >= 0)
        (void)!write(sFd, s, strlen(s));
}

_Unwind_Reason_Code UnwindCb(struct _Unwind_Context* ctx, void* arg) {
    auto* st = static_cast<BtState*>(arg);
    uintptr_t pc = _Unwind_GetIP(ctx);
    if (pc == 0 || st->count >= 48)
        return _URC_END_OF_STACK;

    char line[600];
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(pc), &info) && info.dli_fname != nullptr) {
        uintptr_t off = pc - reinterpret_cast<uintptr_t>(info.dli_fbase);
        const char* lib = strrchr(info.dli_fname, '/');
        lib = lib ? lib + 1 : info.dli_fname;
        snprintf(line, sizeof(line), "#%02d pc %016lx  %s  (%s+%lu)\n", st->count, (unsigned long)off, lib,
                 info.dli_sname ? info.dli_sname : "?",
                 info.dli_saddr ? (unsigned long)(pc - reinterpret_cast<uintptr_t>(info.dli_saddr)) : 0UL);
    } else {
        snprintf(line, sizeof(line), "#%02d pc %016lx  ?\n", st->count, (unsigned long)pc);
    }
    WriteStr(line);
    st->count++;
    return _URC_NO_REASON;
}

void Handler(int sig, siginfo_t* info, void*) {
    sFd = open(sCrashPath, O_WRONLY | O_CREAT | O_APPEND, 0644);
    char head[200];
    snprintf(head, sizeof(head), "\n==== CRASH signal %d code %d fault_addr %p ====\n", sig, info ? info->si_code : 0,
             info ? info->si_addr : nullptr);
    WriteStr(head);
    BtState st{ 0 };
    _Unwind_Backtrace(UnwindCb, &st);
    if (sFd >= 0)
        close(sFd);

    // devolve ao tratador padrão (gera o tombstone normal e fecha o app)
    signal(sig, SIG_DFL);
    raise(sig);
}
} // namespace

extern "C" void ZoInstallCrashLogger(const char* dir) {
    if (dir == nullptr)
        return;
    snprintf(sCrashPath, sizeof(sCrashPath), "%s/crash_native.log", dir);

    static char sAltStack[64 * 1024];
    stack_t ss;
    ss.ss_sp = sAltStack;
    ss.ss_size = sizeof(sAltStack);
    ss.ss_flags = 0;
    sigaltstack(&ss, nullptr);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = Handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    const int sigs[] = { SIGSEGV, SIGABRT, SIGBUS, SIGFPE, SIGILL };
    for (int s : sigs)
        sigaction(s, &sa, nullptr);
}
#else
extern "C" void ZoInstallCrashLogger(const char*) {}
#endif
