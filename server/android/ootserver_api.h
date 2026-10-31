// API C para o servidor ZeldaOnline embutido no app do jogo (libootserver.so).
// Só estas funções ficam visíveis; o resto do servidor é oculto (-fvisibility=hidden)
// para não conflitar com as classes de mesmo nome do cliente dentro de libsoh.so.
#pragma once

#if defined(__GNUC__)
#define ZO_SERVER_API __attribute__((visibility("default")))
#else
#define ZO_SERVER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Resultado de ZoServer_Start
enum {
    ZO_SERVER_OK = 0,
    ZO_SERVER_BAD_PORT = 1,
    ZO_SERVER_ALREADY_RUNNING = 2,
    ZO_SERVER_CONFIG_WRITE_FAILED = 3,
    ZO_SERVER_THREAD_FAILED = 4,
};

// Resultado de ZoServer_Status
enum {
    ZO_SERVER_STOPPED = 0,
    ZO_SERVER_RUNNING = 1,
    ZO_SERVER_ERROR = 2,    // veja ZoServer_LastError
    ZO_SERVER_STARTING = 3, // thread iniciada, porta ainda não aberta
};

// Resultado de ZoServer_LastError (quando o status é ZO_SERVER_ERROR)
enum {
    ZO_SERVER_ERR_NONE = 0,
    ZO_SERVER_ERR_SOCKET = 1,  // não conseguiu criar o socket
    ZO_SERVER_ERR_LISTEN = 2,  // porta ocupada ou sem permissão
    ZO_SERVER_ERR_THREAD = 3,
};

// Inicia o servidor na porta TCP indicada. configPath é o caminho do config.json; se o
// arquivo não existir, é criado com a configuração padrão embutida.
ZO_SERVER_API int ZoServer_Start(int port, const char* configPath);
ZO_SERVER_API void ZoServer_Stop(void);
ZO_SERVER_API int ZoServer_Status(void);
ZO_SERVER_API int ZoServer_LastError(void);
ZO_SERVER_API int ZoServer_Port(void);

#ifdef __cplusplus
}
#endif
