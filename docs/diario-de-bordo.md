# Diário de bordo
## Missão M01 — 11/11
- Integrantes e papéis: Adrya (codificou) e Hagata (revisou).
- Objetivo: implementar `musica_criar` e `musica_destruir` em `src/musica.c`, com alocação dinâmica, cópia segura de strings e liberação de memória, para fazer `test_musica` passar.
- O que implementamos: `musica_criar` aloca uma `t_musica` com `malloc`, copia título e artista e guarda id e duração. Devolve `NULL` se título/artista forem `NULL`, se a duração for negativa ou se o `malloc` falhar. `musica_destruir` recebe `void *` e chama `free` (aceita `NULL`).
- Decisão técnica e justificativa: a cópia dos textos usa `strncpy` com terminador `'\0'` explícito na última posição, para nunca estourar os buffers de 80 caracteres. `musica_destruir` recebe `void *` para poder ser usada depois como callback de `lse_destruir`, já que a lista guarda cargas genéricas.
- Problema encontrado e solução: (1) o compilador avisou `-Wnewline-eof` por falta de quebra de linha no fim de `src/musica.c`; corrigido adicionando a linha. (2) Tentamos rodar `make` dentro de `src/` e `tests/` e deu "No rule to make target"; o `Makefile` fica na raiz `miniplayer/`, então os comandos devem ser rodados de lá.
- Evidências de testes: `make build/test_musica && ./build/test_musica` imprimiu `test_musica: OK`. (Colar aqui o print ou a saída do terminal.)
- Próxima ação: M02 — contratos dos TADs e `lse_criar`, `lse_tamanho`, `lse_primeiro` e `lse_destruir` em `src/lse.c`, com os papéis invertidos.


## Missão M02 — 13/11
- Integrantes e papéis: Adrya (codificou) e Hagata (revisou).
- Objetivo: implementar a lista vazia em `src/lse.c` (`lse_criar`, `lse_tamanho`, `lse_primeiro`, `lse_destruir`) e fazer o teste da M02 passar sem alterar o teste.
- O que implementamos: `lse_criar` aloca a lista com `inicio == NULL` e `tamanho == 0`; `lse_tamanho` e `lse_primeiro` são consultas sem efeito colateral; `lse_destruir` percorre e libera os nós e a lista, chamando o callback de carga só se não for `NULL`.
- Decisão técnica e justificativa: manter `t_elem` interno ao `lse.c` e a carga como `void *`, para pilha e fila reutilizarem a lista sem conhecer sua representação. Tratamos `NULL` nas consultas e na destruição para evitar acessos inválidos.
- Problema encontrado e solução: as pastas das missões estavam na raiz do projeto, mas os Makefiles incrementais esperam `missoes/Mxx/`; movemos `M02` e `M03` para `missoes/` e acrescentamos o bloco de integração ao `Makefile`, conforme o guia do professor.
- Evidências de testes: antes de implementar, `make test_m02` falhou com os stubs; depois, imprimiu `M02: teste de contrato passou.` e `test_musica: OK` continuou passando. (Colar as saídas reais.)
- Próxima ação: M03 — percurso da LSE (`make test_m03`), com os papéis invertidos de novo.

> Duplicar esta seção a cada missão e alternar os papéis na dupla.


## Missão M03 — 16/11
- Integrantes e papéis: Hagata (codificou) e Adrya (revisou).
- Objetivo: implementar o encadeamento e o percurso da lista em `src/lse.c`: `lse_inserir_inicio`, `lse_percorrer` (novo, com `t_lse_visitar` em `lse.h`) e revisar `lse_destruir`, fazendo `make test_m03` passar.
- O que implementamos: `criar_no` (função interna que aloca um nó), `lse_inserir_inicio` (liga o novo nó ao antigo primeiro e só então atualiza `inicio`; incrementa `tamanho`), `lse_percorrer` (laço de `inicio` até `NULL` chamando o callback com a carga). Completamos os testes do esqueleto (`test_m03.c`): lista vazia, 1 e 3 elementos (ordem inversa à inserção), carga `NULL`, cargas `int` e `t_musica`, destruição com callback e `lse_inserir_inicio(NULL, ...)`.
- Decisão técnica e justificativa: aceitar carga `NULL` (um nó ainda é um nó), registrando que `lse_primeiro` fica ambíguo; `t_elem` permanece em `lse.c`; o callback recebe só a carga.
- Problema encontrado e solução: (1) o enunciado declara `lse_percorrer(..., t_visitar)` mas o `typedef` se chama `t_lse_visitar`; usamos `t_lse_visitar` nos dois lugares. (2) Os testes do kit começam com `assert(1==0)` de propósito, precisam ser preenchidos pela dupla.
- Evidências de testes: `make test_m03` imprimiu `M03: todos os testes passaram.`
- Próxima ação: M04 — `lse_inserir_fim`, `lse_buscar`, `lse_remover` e `lse_remover_inicio`.


## Missão M04 — 18/11
- Integrantes e papéis: Hagata (codificou) e Adrya (revisou).
- Objetivo: implementar inserção no fim, busca por chave com callback e remoção (por chave e do início), sem regressão da M01–M03.
- O que implementamos: `lse_inserir_fim` (percorre até o último nó, O(n)), `lse_buscar` (devolve a primeira carga com `comparar == 0`), `lse_remover` (mantém `anterior` e `atual`; trata cabeça, meio e cauda; libera só o nó e devolve a carga) e `lse_remover_inicio`. Completamos `test_m04.c`: inserção em lista vazia e não vazia, busca de primeiro/meio/último/ausente, remoção de meio/fim/início/único/inexistente, chave duplicada, remoção seguida de reinserção.
- Decisão técnica e justificativa: `lse_remover` não libera a carga porque a lista não sabe quem é o dono dela; a biblioteca chama `musica_destruir`. Cargas `NULL` não são usadas em busca/remoção por serem ambíguas com "não encontrado".
- Problema encontrado e solução: (1) `test_m04.c` do kit usava `t_musica` sem declará-lo e não compilava; trocamos por `musica_t`, o tipo definido no próprio teste e usado pelo comparador (nenhuma asserção foi alterada para mascarar defeito). (2) O enunciado cita `t_comparador_lse`, mas o `lse.h` usa `t_comparador`; mantivemos `t_comparador`.
- Evidências de testes: `make test_m04` imprimiu `M04: todos os testes passaram.`; `make test_missoes` executou M02, M03 e M04.
- Próxima ação: M05 — `lse_inserir_ordenado` e ampliação dos testes.
