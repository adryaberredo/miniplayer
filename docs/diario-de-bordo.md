# Diário de bordo
## Missão M01 — 11/11
- Integrantes e papéis: Adrya (codificou) e Hagata (revisou). Papéis serão alternados na M02.
- Objetivo: implementar `musica_criar` e `musica_destruir` em `src/musica.c`, com alocação dinâmica, cópia segura de strings e liberação de memória, para fazer `test_musica` passar.
- O que implementamos: `musica_criar` aloca uma `t_musica` com `malloc`, copia título e artista e guarda id e duração. Devolve `NULL` se título/artista forem `NULL`, se a duração for negativa ou se o `malloc` falhar. `musica_destruir` recebe `void *` e chama `free` (aceita `NULL`).
- Decisão técnica e justificativa: a cópia dos textos usa `strncpy` com terminador `'\0'` explícito na última posição, para nunca estourar os buffers de 80 caracteres. `musica_destruir` recebe `void *` para poder ser usada depois como callback de `lse_destruir`, já que a lista guarda cargas genéricas.
- Problema encontrado e solução: (1) o compilador avisou `-Wnewline-eof` por falta de quebra de linha no fim de `src/musica.c`; corrigido adicionando a linha. (2) Tentamos rodar `make` dentro de `src/` e `tests/` e deu "No rule to make target"; o `Makefile` fica na raiz `miniplayer/`, então os comandos devem ser rodados de lá.
- Evidências de testes: `make build/test_musica && ./build/test_musica` imprimiu `test_musica: OK`. (Colar aqui o print ou a saída do terminal.)
- Próxima ação: M02 — contratos dos TADs e `lse_criar`, `lse_tamanho`, `lse_primeiro` e `lse_destruir` em `src/lse.c`, com os papéis invertidos.

> Duplicar esta seção a cada missão e alternar os papéis na dupla.