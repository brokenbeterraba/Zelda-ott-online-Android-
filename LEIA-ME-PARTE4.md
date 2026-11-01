# Parte 4 — corrige os `undefined reference` do link

Sobre o código-fonte "revisado" (Zelda-ott-online-Android--main).
- 15 atores: nomes `func_XXXXXXXX` renomeados para os nomes reais (EnBrob_Idle, EnReeba_*, EnRd_*, EnDodojr_*, BgBdanObjects_*, ...),
  mapeados por posição das definições contra o ShipwrightOnline (mesma decomp, só os nomes mudam). Total: 225 nomes.
- z_boss_sst.c: adicionada BossSst_SetFloor (só existia no ShipwrightOnline).
Copie estes arquivos por cima do repositório (mesmos caminhos) e rode o workflow.
Não compilado com NDK; o próximo erro, se houver, aparece na issue "Relatório de build CI".
