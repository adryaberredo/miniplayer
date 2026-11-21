
## M01 — Módulo música (`include/musica.h`, `src/musica.c`)

### musica_criar
- Nome e assinatura: `t_musica *musica_criar(int id, const char *titulo, const char *artista, int duracao)`
- Pré-condições: `titulo` e `artista` não nulos, apontando para strings terminadas em `'\0'`; `duracao >= 0`.
- Pós-condições: em sucesso, existe uma nova `t_musica` no heap com `id` e `duracao` iguais aos recebidos e `titulo`/`artista` copiados para os buffers internos (80 bytes cada). Textos maiores que 79 caracteres são truncados, e o último byte é sempre `'\0'`. As strings originais não são alteradas nem guardadas por ponteiro.
- Valor de retorno em sucesso/falha: sucesso devolve o ponteiro para a nova música; falha devolve `NULL` (título ou artista `NULL`, duração negativa ou falha do `malloc`). Nada é alocado em caso de parâmetro inválido.
- Quem aloca/libera a memória: `musica_criar` aloca. Quem recebe o ponteiro é o dono e deve liberar com `musica_destruir`. No projeto, o dono é a biblioteca.
- Invariantes preservadas: `titulo` e `artista` são sempre strings válidas terminadas em `'\0'`; `duracao >= 0`.
- Testes associados: `tests/test_musica.c` (T01).

### musica_destruir
- Nome e assinatura: `void musica_destruir(void *carga)`
- Pré-condições: `carga` é `NULL` ou um ponteiro devolvido por `musica_criar` que ainda não foi liberado.
- Pós-condições: a memória da música é liberada. O ponteiro não deve mais ser usado. Com `NULL`, nada acontece.
- Valor de retorno em sucesso/falha: não retorna valor.
- Quem aloca/libera a memória: libera a música apontada por `carga`. O parâmetro é `void *` para servir como callback de `lse_destruir`, já que a lista guarda cargas genéricas.
- Invariantes preservadas: não afeta nenhuma outra música nem nenhuma estrutura. Antes de chamar, é preciso remover referências à música de fila e histórico (regra do README).
- Testes associados: `tests/test_musica.c` (T01, que cria e destrói).


## M02 — Lista vazia (`include/lse.h`, `src/lse.c`)

### lse_criar
- Nome e assinatura: `t_lse *lse_criar(void)`
- Pré-condições: nenhuma.
- Pós-condições: existe uma lista vazia no heap, com `inicio == NULL` e `tamanho == 0`.
- Valor de retorno em sucesso/falha: sucesso devolve o ponteiro para a lista; falha de memória devolve `NULL`.
- Quem aloca/libera a memória: `lse_criar` aloca a estrutura da lista; quem a recebe deve liberá-la com `lse_destruir`.
- Invariantes preservadas: `tamanho == 0` se e somente se `inicio == NULL`.
- Testes associados: `missoes/M02/TESTE_INCREMENTAL_M02.c`.

### lse_tamanho
- Nome e assinatura: `int lse_tamanho(const t_lse *lista)`
- Pré-condições: `lista` válida (ou `NULL`, tratada como vazia).
- Pós-condições: a lista não é alterada.
- Valor de retorno em sucesso/falha: número de elementos (0 para lista recém-criada ou `NULL`).
- Quem aloca/libera a memória: não aloca nem libera.
- Invariantes preservadas: `tamanho >= 0`.
- Testes associados: `missoes/M02/TESTE_INCREMENTAL_M02.c`.

### lse_primeiro
- Nome e assinatura: `void *lse_primeiro(const t_lse *lista)`
- Pré-condições: `lista` válida (ou `NULL`).
- Pós-condições: a lista não é alterada; o elemento não é removido.
- Valor de retorno em sucesso/falha: a carga do primeiro nó; `NULL` se a lista for vazia (ou `NULL`).
- Quem aloca/libera a memória: não aloca nem libera; a carga continua pertencendo a quem a inseriu.
- Invariantes preservadas: todas as invariantes da LSE.
- Testes associados: `missoes/M02/TESTE_INCREMENTAL_M02.c`.

### lse_destruir
- Nome e assinatura: `void lse_destruir(t_lse *lista, void (*destruir_carga)(void *))`
- Pré-condições: `lista` é `NULL` ou foi criada por `lse_criar` e ainda não foi destruída.
- Pós-condições: todos os nós e a estrutura da lista são liberados; o ponteiro não deve mais ser usado.
- Valor de retorno em sucesso/falha: não retorna valor; com `NULL`, nada acontece.
- Quem aloca/libera a memória: libera os nós e a lista. As cargas só são liberadas se `destruir_carga` não for `NULL`; com `NULL`, as cargas de terceiros são preservadas.
- Invariantes preservadas: não deixa nós alcançáveis nem vazamentos.
- Testes associados: `missoes/M02/TESTE_INCREMENTAL_M02.c` (lista vazia com `destruir_carga == NULL`).


## M03 — Construção e percurso da lista (`include/lse.h`, `src/lse.c`)

### lse_inserir_inicio
- Nome e assinatura: `int lse_inserir_inicio(t_lse *lista, void *carga)`
- Pré-condições: `lista` criada por `lse_criar` e ainda não destruída. `carga` pode ser `NULL` (decisão da M03).
- Pós-condições: em sucesso, existe um novo nó na cabeça apontando para o antigo primeiro; `inicio` aponta para ele e `tamanho` aumenta em 1. Em falha, a lista permanece exatamente como estava.
- Valor de retorno em sucesso/falha: `1` no sucesso; `0` se `lista == NULL` ou se o `malloc` do nó falhar.
- Quem aloca/libera a memória: a função aloca o **nó**; a carga não é copiada nem alocada (a lista só guarda o ponteiro). O nó é liberado por `lse_remover_inicio`, `lse_remover` ou `lse_destruir`; a carga continua pertencendo a quem a inseriu.
- Invariantes preservadas: `tamanho == 0` sse `inicio == NULL`; nº de nós alcançáveis a partir de `inicio` == `tamanho`; o último nó tem `prox == NULL`; sem ciclos.
- Testes associados: `missoes/M03/test_m03.c` (`teste_encadeamento_e_percurso`, `teste_carga_null`, `teste_carga_generica`, `main`).

### lse_percorrer
- Nome e assinatura: `void lse_percorrer(const t_lse *lista, t_lse_visitar cb_visitar)`, com `typedef void (*t_lse_visitar)(void *carga);`
- Pré-condições: nenhuma obrigatória; `lista` ou `cb_visitar` `NULL` são tratados.
- Pós-condições: o callback é chamado exatamente uma vez por nó, do início ao fim, recebendo a carga. A lista não é alterada. O callback **não deve** inserir nem remover nós durante o percurso.
- Valor de retorno em sucesso/falha: não retorna valor. Lista vazia, `lista == NULL` ou `cb_visitar == NULL`: nenhuma chamada ao callback, sem erro.
- Quem aloca/libera a memória: não aloca nem libera.
- Invariantes preservadas: todas (somente leitura).
- Testes associados: `missoes/M03/test_m03.c` (`teste_lista_vazia`, `teste_encadeamento_e_percurso`).

### lse_destruir (revisão na M03)
- Contrato da M02 mantido: libera todos os nós e a lista; chama `destruir_carga` **uma vez por carga, antes de liberar o nó**, apenas se `destruir_carga != NULL`. Não presume que a carga veio do heap: com `NULL`, cargas automáticas (pilha) ou de terceiros ficam intactas.
- Testes associados: `test_m03.c` (`teste_destruicao_com_callback`: 3 cargas em `malloc`, callback chamado 3 vezes; demais testes destroem com `NULL`).

### Decisão: carga `NULL`
`lse_inserir_inicio(lista, NULL)` é **aceita** e cria um nó (tamanho aumenta, o callback de percurso recebe `NULL`). Consequência: `lse_primeiro` devolve `NULL` tanto para lista vazia quanto para lista cujo primeiro nó guarda carga `NULL`; para distinguir, use `lse_tamanho`. Por isso a M04 proíbe cargas `NULL` em busca/remoção.


## M04 — Inserção no fim, busca e remoção (`include/lse.h`, `src/lse.c`)

### lse_inserir_fim
- Nome e assinatura: `int lse_inserir_fim(t_lse *lista, void *carga)`
- Pré-condições: `lista` válida. A carga **não deve ser `NULL`** nos cenários de busca/remoção (ver ambiguidade abaixo).
- Pós-condições: em sucesso, novo nó é o último; a ordem anterior é preservada; `tamanho` aumenta em 1. Em lista vazia, o novo nó também passa a ser `inicio`. Em falha, a lista não muda.
- Valor de retorno em sucesso/falha: `1` no sucesso; `0` se `lista == NULL` ou `malloc` falhar.
- Quem aloca/libera a memória: aloca o nó; a carga segue com quem a inseriu.
- Custo: O(n), pois ainda não existe ponteiro `fim` (adiado para a M08).
- Invariantes preservadas: as mesmas da M03.
- Testes associados: `missoes/M04/test_m04.c` (`teste_inserir_fim_e_buscar`, `teste_remover_*`).

### lse_buscar
- Nome e assinatura: `void *lse_buscar(const t_lse *lista, const void *chave, t_comparador comparar)`
- Pré-condições: `comparar(carga, chave)` devolve 0 quando a carga corresponde à chave e **não altera** carga nem chave.
- Pós-condições: a lista não é alterada.
- Valor de retorno em sucesso/falha: a **primeira** carga correspondente; `NULL` se não houver, ou se `lista`/`comparar` forem `NULL`.
- Quem aloca/libera a memória: não aloca nem libera; a carga continua na lista.
- Invariantes preservadas: todas (somente leitura).
- Testes associados: `test_m04.c` (`teste_inserir_fim_e_buscar`, `teste_remover_inicio`, `teste_generico_musicas`, `teste_argumentos_invalidos`).

### lse_remover
- Nome e assinatura: `void *lse_remover(t_lse *lista, const void *chave, t_comparador comparar)`
- Pré-condições: as mesmas de `lse_buscar`.
- Pós-condições: remove **apenas o primeiro** nó correspondente, religando o anterior ao seguinte (ou atualizando `inicio` se era a cabeça); `tamanho` diminui em 1. Se não houver correspondência, nada muda.
- Valor de retorno em sucesso/falha: a carga do nó removido; `NULL` se não encontrar ou se `lista`/`comparar` forem `NULL`.
- Quem aloca/libera a memória: libera **somente o nó**. A carga é devolvida a quem chamou, que decide se a libera (ex.: `musica_destruir`).
- Invariantes preservadas: as mesmas da M03; lista que perde o único nó volta a `inicio == NULL` e `tamanho == 0`.
- Testes associados: `test_m04.c` (`teste_remover_cabeca_meio_cauda`, `teste_remover_inicio`, `teste_generico_musicas`).

### lse_remover_inicio
- Nome e assinatura: `void *lse_remover_inicio(t_lse *lista)`
- Pré-condições: nenhuma obrigatória.
- Pós-condições: remove o primeiro nó; `inicio` passa ao segundo; `tamanho` diminui em 1.
- Valor de retorno em sucesso/falha: a carga removida; `NULL` se a lista for `NULL` ou vazia.
- Quem aloca/libera a memória: libera só o nó; a carga é devolvida.
- Invariantes preservadas: as mesmas da M03.
- Testes associados: `test_m04.c` (`teste_remover_inicio`, `teste_argumentos_invalidos`); `tests/test_lse.c`.

### Ambiguidade da carga `NULL`
`NULL` significa "ausente" em `lse_buscar`, `lse_remover` e `lse_remover_inicio`; portanto, uma carga `NULL` legítima seria indistinguível de "não encontrado". Solução futura possível: devolver um código de status (`int`) e entregar a carga por parâmetro de saída, por exemplo `int lse_remover(t_lse *l, const void *chave, t_comparador cmp, void **carga_removida)`, ou simplesmente proibir `NULL` como carga em `lse_inserir_*`.
