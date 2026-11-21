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

/* M03: cria um no isolado (prox == NULL). Funcao interna: a aplicacao
 * nunca ve t_elem. O no guarda o ponteiro da carga; nao copia a carga. */
static t_elem *criar_no(void *carga) {
    t_elem *n = malloc(sizeof *n);
    if (n == NULL) return NULL;
    n->carga = carga;
    n->prox = NULL;
    return n;
}

/* M03: insere no inicio em O(1). Retorna 1 no sucesso; 0 se lista == NULL
 * ou malloc falhar (lista intacta). Carga NULL e permitida. */
int lse_inserir_inicio(t_lse *l, void *carga) {
    if (l == NULL) return 0;
    t_elem *n = criar_no(carga);
    if (n == NULL) return 0;
    n->prox = l->inicio;   /* 1) novo no aponta para o antigo primeiro */
    l->inicio = n;         /* 2) inicio passa a ser o novo no */
    l->tamanho++;
    return 1;
}

/* M03: visita as cargas do inicio ao fim. Nao faz nada se lista ou
 * callback forem NULL. Nao altera a lista. */
void lse_percorrer(const t_lse *l, t_lse_visitar visitar) {
    if (l == NULL || visitar == NULL) return;
    for (t_elem *atual = l->inicio; atual != NULL; atual = atual->prox) {
        visitar(atual->carga);
    }
}

/* M04: insere no fim em O(n) (ainda nao ha ponteiro fim). Retorna 1 no
 * sucesso; 0 se lista == NULL ou malloc falhar. */
int lse_inserir_fim(t_lse *l, void *carga) {
    if (l == NULL) return 0;
    t_elem *n = criar_no(carga);
    if (n == NULL) return 0;
    if (l->inicio == NULL) {
        l->inicio = n;                 /* lista vazia: o no e o primeiro */
    } else {
        t_elem *ultimo = l->inicio;
        while (ultimo->prox != NULL) ultimo = ultimo->prox;
        ultimo->prox = n;
    }
    l->tamanho++;
    return 1;
}

/* M04: remove o primeiro no e devolve a carga (so o no e liberado).
 * NULL se a lista for NULL ou vazia. */
void *lse_remover_inicio(t_lse *l) {
    if (l == NULL || l->inicio == NULL) return NULL;
    t_elem *no = l->inicio;
    void *carga = no->carga;
    l->inicio = no->prox;
    free(no);
    l->tamanho--;
    return carga;
}

/* M04: devolve a primeira carga com comparar(carga, chave) == 0.
 * NULL se nao achar ou se lista/comparar forem NULL. Nao altera a lista. */
void *lse_buscar(const t_lse *l, const void *chave, t_comparador comparar) {
    if (l == NULL || comparar == NULL) return NULL;
    for (t_elem *atual = l->inicio; atual != NULL; atual = atual->prox) {
        if (comparar(atual->carga, chave) == 0) return atual->carga;
    }
    return NULL;
}

/* M04: remove so o PRIMEIRO no correspondente; libera apenas o no e
 * devolve a carga (quem chamou decide quando liberar a carga).
 * NULL se nao achar ou se lista/comparar forem NULL. */
void *lse_remover(t_lse *l, const void *chave, t_comparador comparar) {
    if (l == NULL || comparar == NULL) return NULL;
    t_elem *anterior = NULL;
    t_elem *atual = l->inicio;
    while (atual != NULL && comparar(atual->carga, chave) != 0) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) return NULL;            /* chave inexistente */
    if (anterior == NULL) l->inicio = atual->prox;   /* removeu a cabeca */
    else anterior->prox = atual->prox;               /* meio ou cauda */
    void *carga = atual->carga;
    free(atual);
    l->tamanho--;
    return carga;
}

/* Stub intencionalmente incompleto; pertence a M05. */
int lse_inserir_ordenado(t_lse *l, void *c, t_comparador cmp) { (void)l; (void)c; (void)cmp; return 0; } /* TODO M05 */
