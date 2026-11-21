#include "lse.h"
#include <stddef.h>
#include <stdlib.h>
/* Representacao interna: NAO mover para lse.h. */
typedef struct elem {
    void *carga;
    struct elem *prox;
} t_elem;
struct lse {
    t_elem *inicio;
    int tamanho;
    /* M08 (otimizacao opcional): t_elem *fim; */
};

/* M02: cria lista vazia. Retorna NULL se malloc falhar.
 * Quem recebe a lista deve liberar com lse_destruir. */
t_lse *lse_criar(void) {
    t_lse *l = malloc(sizeof *l);
    if (l == NULL) return NULL;
    l->inicio = NULL;
    l->tamanho = 0;
    return l;
}

/* M02: numero de elementos (0 para lista vazia ou NULL). */
int lse_tamanho(const t_lse *l) {
    if (l == NULL) return 0;
    return l->tamanho;
}

/* M02: carga do primeiro no, sem remove-lo. NULL se vazia. */
void *lse_primeiro(const t_lse *l) {
    if (l == NULL || l->inicio == NULL) return NULL;
    return l->inicio->carga;
}

/* M02: libera os nos e a lista. As cargas so sao liberadas se
 * destruir nao for NULL. */
void lse_destruir(t_lse *l, void (*destruir)(void *)) {
    if (l == NULL) return;
    t_elem *atual = l->inicio;
    while (atual != NULL) {
        t_elem *prox = atual->prox;
        if (destruir != NULL) destruir(atual->carga);
        free(atual);
        atual = prox;
    }
    free(l);
}

/* Stubs intencionalmente incompletos; testes devem falhar ate implementacao. */
int lse_inserir_inicio(t_lse *l, void *c) { (void)l; (void)c; return 0; } /* TODO M04 */
int lse_inserir_fim(t_lse *l, void *c) { (void)l; (void)c; return 0; } /* TODO M04 */
void *lse_remover_inicio(t_lse *l) { (void)l; return NULL; } /* TODO M04 */
void *lse_buscar(const t_lse *l, const void *ch, t_comparador cmp) { (void)l; (void)ch; (void)cmp; return NULL; } /* TODO M04 */
void *lse_remover(t_lse *l, const void *ch, t_comparador cmp) { (void)l; (void)ch; (void)cmp; return NULL; } /* TODO M04 */
int lse_inserir_ordenado(t_lse *l, void *c, t_comparador cmp) { (void)l; (void)c; (void)cmp; return 0; } /* TODO M05 */

