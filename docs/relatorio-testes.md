# Relatório de testes
| ID | Missão | Função/contrato | Entrada/cenário | Resultado esperado | Resultado obtido | Status |
|---|---|---|---|---|---|---|
| T01 | M01 | musica_criar | dados válidos (1, "Teste", "Artista", 180) | ponteiro não nulo e campos corretos | ponteiro não nulo, campos corretos; `test_musica: OK` | passou |
| T02 | M01 | musica_criar | título NULL | retorna NULL | retornou NULL | passou |
| T03 | M01 | musica_criar | artista NULL | retorna NULL | retornou NULL | passou |
| T04 | M01 | musica_criar | duração negativa (-1) | retorna NULL | retornou NULL | passou |
| T05 | M01 | musica_criar | título com 199 caracteres (maior que o buffer de 80) | título truncado em 79 caracteres, terminado em '\0', sem estouro | strlen = 79, sem erro no AddressSanitizer | passou |
| T06 | M01 | musica_criar | duração 0 | aceita e guarda 0 | música criada com duração 0 | passou |
| T07 | M01 | musica_destruir | ponteiro NULL | não faz nada, sem falha | sem falha | passou |
| T08 | M01 | musica_destruir | música válida | libera memória sem vazamento | sem erros no AddressSanitizer | passou |
| T09 | M02 | lse_criar | chamar `lse_criar()` com stubs originais | lista válida (não nula) | falhou: `lse_criar` devolvia NULL (antes de implementar) | falhou (esperado) |
| T10 | M02 | lse_criar / lse_tamanho / lse_primeiro / lse_destruir | lista recém-criada; tamanho 0; primeiro NULL; destruir sem callback | lista não nula, tamanho 0, primeiro NULL, destruição sem erro | `M02: teste de contrato passou.` | passou |
| T11 | M02 | regressão M01 | `make build/test_musica && ./build/test_musica` após alterar `lse.c` | `test_musica: OK` | `test_musica: OK` | passou |
| T12 | M03 | lse_tamanho / lse_primeiro / lse_percorrer | lista vazia | tamanho 0, primeiro NULL, 0 visitas | conforme o esperado | passou |
| T13 | M03 | lse_percorrer | `lista == NULL` e `callback == NULL` | não faz nada, sem falha | sem falha | passou |
| T14 | M03 | lse_inserir_inicio | 1 elemento (int) | tamanho 1, primeiro correto, 1 visita | conforme o esperado | passou |
| T15 | M03 | lse_inserir_inicio | 3 elementos a, b, c | percurso c, b, a (LIFO); primeiro = c; tamanho 3 | conforme o esperado | passou |
| T16 | M03 | lse_inserir_inicio | carga NULL | retorna 1; tamanho 1; callback chamado 1 vez com NULL | conforme o esperado | passou |
| T17 | M03 | lse_inserir_inicio | cargas `int` e `t_musica` | mesma lista funciona para ambos os tipos | conforme o esperado | passou |
| T18 | M03 | lse_destruir | 3 cargas no heap + callback `free` | callback chamado 3 vezes; sem vazamento | `destruidos == 3`; sem erros no AddressSanitizer | passou |
| T19 | M03 | lse_inserir_inicio | `lista == NULL` | retorna 0, sem acesso inválido | retornou 0 | passou |
| T20 | M04 | lse_inserir_fim | lista vazia e depois não vazia (a, b, c) | ordem a, b, c; tamanho 3 | conforme o esperado | passou |
| T21 | M04 | lse_buscar | chave do primeiro, do meio e do último | devolve a carga correta (o ponteiro da lista) | conforme o esperado | passou |
| T22 | M04 | lse_buscar | chave inexistente; verificar lista após buscas | NULL; lista inalterada | conforme o esperado | passou |
| T23 | M04 | lse_remover | meio, fim e início | devolve a carga; religa; tamanho correto a cada passo | conforme o esperado | passou |
| T24 | M04 | lse_remover | único elemento | devolve a carga; lista vazia (primeiro NULL, tamanho 0) | conforme o esperado | passou |
| T25 | M04 | lse_remover | chave inexistente / lista vazia | NULL; lista inalterada | conforme o esperado | passou |
| T26 | M04 | lse_remover | chave duplicada (5, 5, 8) | remove só a primeira ocorrência; a segunda ainda é encontrada | conforme o esperado | passou |
| T27 | M04 | lse_remover_inicio | lista vazia; com elementos até esvaziar | NULL na vazia; devolve cargas na ordem; ao final NULL | conforme o esperado | passou |
| T28 | M04 | lse_remover + lse_inserir_* | remover e reinserir (fim e início) | ordem e tamanho corretos | conforme o esperado | passou |
| T29 | M04 | lse_buscar / lse_remover | cargas `t_musica`-like comparadas por `id` | achou e removeu a música 22; sobrou a 11 | conforme o esperado | passou |
| T30 | M04 | todas | lista NULL, callback NULL, lista vazia | NULL/0 sem acesso inválido; lista inalterada | conforme o esperado | passou |
| T31 | M03/M04 | regressão M01–M02 | `make test_m02`, `test_musica`, `test_lse` | continuam passando | `M02: teste de contrato passou.`, `test_musica: OK`, `test_lse: OK` | passou |

## Casos-limite
- Estrutura vazia: não se aplica à M01 (ainda não há lista). `musica_destruir(NULL)` é seguro (T07).
- Estrutura com um elemento: uma única música criada e destruída sem vazamento (T01, T08).
- Chave inexistente: não se aplica à M01 (busca só a partir da M04). Entradas inválidas (título/artista NULL, duração negativa) devolvem NULL (T02 a T04).
- Falha de alocação (discutir estratégia de simulação): se `malloc` devolver NULL, `musica_criar` retorna NULL e não altera nada. Para simular, pode-se compilar um teste com um `malloc` substituto que sempre devolve NULL, ou envolver a alocação em uma função que o teste consiga forçar a falhar.

## Memória
- Compilação: `make build/test_musica && ./build/test_musica` (gcc, `-std=c11 -Wall -Wextra -Wpedantic -g`).
- Verificação extra: compilado com `-fsanitize=address,undefined` para T02 a T08, sem erros nem vazamentos.
- Problemas corrigidos: aviso `-Wnewline-eof` (faltava quebra de linha no fim de `src/musica.c`); cópia de strings feita com `strncpy` mais terminador '\0' explícito para evitar estouro do buffer.


## M02 — Memória
- Comando: `make test_m02` (a partir da raiz `miniplayer/`).
- Antes: teste falhou por `lse_criar` devolver NULL. Depois: teste passou.
- Casos-limite desta missão: lista vazia (tamanho 0, primeiro NULL) e destruição de lista vazia sem callback.


## M03 — Casos-limite e memória
- Estrutura vazia: tamanho 0, `lse_primeiro == NULL`, percurso sem visitas (T12).
- Um elemento: T14. Três elementos: T15 (ordem inversa à inserção).
- Entradas inválidas: `lse_inserir_inicio(NULL, ...)` (T19); `lse_percorrer` com lista ou callback `NULL` (T13).
- Carga `NULL`: aceita (T16); `lse_primeiro` é ambíguo nesse caso, daí o uso de `lse_tamanho`.
- Comando: `make test_m03`. Verificação extra: compilado com `-fsanitize=address,undefined`, sem erros nem vazamentos.
- Problemas: o kit trazia o enunciado com `t_visitar` e o `typedef` como `t_lse_visitar` (unificamos); os testes começavam com `assert(1==0)` e foram completados.

## M04 — Casos-limite e memória
- Estrutura vazia: remoção e busca devolvem `NULL` (T25, T27). Um elemento: remoção deixa a lista vazia (T24). Chave inexistente: T22 e T25. Chave duplicada: T26.
- Entradas inválidas: lista `NULL`, callback `NULL` (T30).
- Comando: `make test_m04`; regressão: `make test_missoes`. Verificação extra com `-fsanitize=address,undefined`, sem erros nem vazamentos.
- Validação dos testes: introduzimos defeitos de propósito em `lse.c` (não decrementar `tamanho` na remoção, não atualizar `inicio` ao remover a cabeça, não incrementar `tamanho` no fim, buscar o último em vez do primeiro, percurso que ignora o último nó, inserção que não liga ao antigo primeiro); os testes de M03/M04 detectaram todos.
- Problemas corrigidos: `test_m04.c` usava `t_musica` sem declaração (trocado por `musica_t`, do próprio teste).
