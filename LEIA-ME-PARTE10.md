# Parte 10 - inimigos sincronizados (spawn pelo cliente)
Copie por cima do repositorio (mesmos caminhos), num commit so.
- z_actor.c: Actor_Spawn e Actor_SpawnAsChild passam pelo cliente online (como no fork); flag de limpeza de sala/objetos; Actor_SpawnOriginal e o spawn real
- functions.h: prototipo de Actor_SpawnOriginal
- code_800F7260.c: sons de atores sincronizados
- ZeldaOnlineClient.cpp: guarda quando o cliente ainda nao existe
