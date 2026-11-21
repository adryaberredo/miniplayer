/* Testes aditivos da M03. Executar a partir da raiz com o Makefile incremental. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "lse.h"
#include "musica.h"   /* so para o tipo t_musica (carga de outro tipo); nao liga musica.c */

/* Estado compartilhado com os callbacks (a lista nao conhece estes tipos). */
static void *esperados[8];
static int visitas;
static int destruidos;

/* Callback de percurso: confere que a carga visitada e a esperada, na ordem. */
static void verificar_ordem(void *carga) {
    assert(visitas < 8);
    assert(carga == esperados[visitas]);
    visitas++;
}

/* Callback destrutor: so para cargas alocadas com malloc. */
static void destruir_int(void *carga) {
    free(carga);
    destruidos++;
}

/* Percorre a lista e confere tamanho, numero de visitas e ordem. */
static void conferir(const t_lse *l, int n) {
    assert(lse_tamanho(l) == n);
    visitas = 0;
    lse_percorrer(l, verificar_ordem);
    assert(visitas == n);
}

static void teste_lista_vazia(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);
    assert(lse_tamanho(l) == 0);
    assert(lse_primeiro(l) == NULL);
    conferir(l, 0);                       /* percurso sem nenhuma visita */
    lse_percorrer(NULL, verificar_ordem); /* lista NULL: nao faz nada */
    lse_percorrer(l, NULL);               /* callback NULL: nao faz nada */
    assert(visitas == 0);
    lse_destruir(l, NULL);
    lse_destruir(NULL, NULL);             /* destruir NULL e seguro */
}

static void teste_encadeamento_e_percurso(void) {
    int a = 1, b = 2, c = 3;
    t_lse *l = lse_criar();
    assert(l != NULL);

    /* um elemento */
    assert(lse_inserir_inicio(l, &a) == 1);
    assert(lse_primeiro(l) == &a);
    esperados[0] = &a;
    conferir(l, 1);

    /* dois e tres elementos: LIFO na cabeca (ultimo inserido vem primeiro) */
    assert(lse_inserir_inicio(l, &b) == 1);
    assert(lse_primeiro(l) == &b);
    esperados[0] = &b; esperados[1] = &a;
    conferir(l, 2);

    assert(lse_inserir_inicio(l, &c) == 1);
    assert(lse_primeiro(l) == &c);
    esperados[0] = &c; esperados[1] = &b; esperados[2] = &a;
    conferir(l, 3);

    lse_destruir(l, NULL);                /* cargas na pilha: nao liberar */
}

static void teste_carga_null(void) {
    /* Decisao M03: carga NULL e aceita (um no ainda e um no). */
    t_lse *l = lse_criar();
    assert(l != NULL);
    assert(lse_inserir_inicio(l, NULL) == 1);
    assert(lse_tamanho(l) == 1);
    assert(lse_primeiro(l) == NULL);      /* ambiguo com "lista vazia"! */
    esperados[0] = NULL;
    conferir(l, 1);                       /* mas o callback e chamado 1 vez */
    lse_destruir(l, NULL);
}

static void teste_carga_generica(void) {
    /* mesma lista, cargas de tipos diferentes (em listas separadas) */
    int n1 = 10, n2 = 20;
    t_lse *li = lse_criar();
    assert(li != NULL);
    assert(lse_inserir_inicio(li, &n1) == 1);
    assert(lse_inserir_inicio(li, &n2) == 1);
    assert(*(int *)lse_primeiro(li) == 20);
    esperados[0] = &n2; esperados[1] = &n1;
    conferir(li, 2);
    lse_destruir(li, NULL);

    t_musica m1 = {1, "Titulo A", "Artista A", 180};
    t_musica m2 = {2, "Titulo B", "Artista B", 200};
    t_lse *lm = lse_criar();
    assert(lm != NULL);
    assert(lse_inserir_inicio(lm, &m1) == 1);
    assert(lse_inserir_inicio(lm, &m2) == 1);
    assert(((t_musica *)lse_primeiro(lm))->id == 2);
    esperados[0] = &m2; esperados[1] = &m1;
    conferir(lm, 2);
    lse_destruir(lm, NULL);
}

static void teste_destruicao_com_callback(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);
    for (int i = 0; i < 3; i++) {
        int *p = malloc(sizeof *p);
        assert(p != NULL);
        *p = i;
        assert(lse_inserir_inicio(l, p) == 1);
    }
    assert(lse_tamanho(l) == 3);
    destruidos = 0;
    lse_destruir(l, destruir_int);        /* callback uma vez por carga */
    assert(destruidos == 3);
}

int main(void) {
    teste_lista_vazia();
    teste_encadeamento_e_percurso();
    teste_carga_null();
    teste_carga_generica();
    teste_destruicao_com_callback();
    assert(lse_inserir_inicio(NULL, NULL) == 0);
    puts("M03: todos os testes passaram.");
    return 0;
}
