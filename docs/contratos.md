
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