# Diário de bordo
## Missão M01 — 11/11
- Integrantes e papéis: Adrya (codificou) e Hagata (revisou). Papéis serão alternados na M02.
- Objetivo: implementar `musica_criar` e `musica_destruir` em `src/musica.c`, com alocação dinâmica, cópia segura de strings e liberação de memória, para fazer `test_musica` passar.
- O que implementamos: `musica_criar` aloca uma `t_musica` com `malloc`, copia título e artista e guarda id e duração. Devolve `NULL` se título/artista forem `NULL`, se a duração for negativa ou se o `malloc` falhar. `musica_destruir` recebe `void *` e chama `free` (aceita `NULL`).
- Decisão técnica e justificativa: a cópia dos textos usa `strncpy` com terminador `'\0'` explícito na última posição, para nunca estourar os buffers de 80 caracteres. `musica_destruir` recebe `void *` para poder ser usada depois como callback de `lse_destruir`, já que a lista guarda cargas genéricas.
- Problema encontrado e solução: (1) o compilador avisou `-Wnewline-eof` por falta de quebra de linha no fim de `src/musica.c`; corrigido adicionando a linha. (2) Tentamos rodar `make` dentro de `src/` e `tests/` e deu "No rule to make target"; o `Makefile` fica na raiz `miniplayer/`, então os comandos devem ser rodados de lá.
- Evidências de testes: `make build/test_musica && ./build/test_musica` imprimiu `test_musica: OK`. (Colar aqui o print ou a saída do terminal.)
- Próxima ação: M02 — contratos dos TADs e `lse_criar`, `lse_tamanho`, `lse_primeiro` e `lse_destruir` em `src/lse.c`, com os papéis invertidos.


## Missão M02 — 13/11
- Integrantes e papéis: Adrya (codificou) e Hagata (revisou). Papéis invertidos em relação à M01.
- Objetivo: implementar a lista vazia em `src/lse.c` (`lse_criar`, `lse_tamanho`, `lse_primeiro`, `lse_destruir`) e fazer o teste da M02 passar sem alterar o teste.
- O que implementamos: `lse_criar` aloca a lista com `inicio == NULL` e `tamanho == 0`; `lse_tamanho` e `lse_primeiro` são consultas sem efeito colateral; `lse_destruir` percorre e libera os nós e a lista, chamando o callback de carga só se não for `NULL`.
- Decisão técnica e justificativa: manter `t_elem` interno ao `lse.c` e a carga como `void *`, para pilha e fila reutilizarem a lista sem conhecer sua representação. Tratamos `NULL` nas consultas e na destruição para evitar acessos inválidos.
- Problema encontrado e solução: as pastas das missões estavam na raiz do projeto, mas os Makefiles incrementais esperam `missoes/Mxx/`; movemos `M02` e `M03` para `missoes/` e acrescentamos o bloco de integração ao `Makefile`, conforme o guia do professor.
- Evidências de testes: antes de implementar, `make test_m02` falhou com os stubs; depois, imprimiu `M02: teste de contrato passou.` e `test_musica: OK` continuou passando. (Colar as saídas reais.)
- Próxima ação: M03 — percurso da LSE (`make test_m03`), com os papéis invertidos de novo.

> Duplicar esta seção a cada missão e alternar os papéis na dupla.