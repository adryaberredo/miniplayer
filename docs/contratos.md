
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