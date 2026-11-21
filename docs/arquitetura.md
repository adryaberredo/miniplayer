# Arquitetura
## Componentes
| Módulo | Responsabilidade | Dependências |
|---|---|---|
| musica | Dados da música | biblioteca padrão |
| lse | Nós genéricos e encadeamento | biblioteca padrão |
| pilha | Política LIFO | lse |
| fila | Política FIFO | lse |
| miniplayer | Simulação e integração | demais TADs |

## Diagrama de dependências
MiniPlayer → (LSE, Pilha, Fila) → LSE → t_elem interno.

## Decisões técnicas
- Por que `void *`? Para a lista (LSE) ser genérica: ela guarda um ponteiro para qualquer carga e não conhece `t_musica`. Assim a mesma LSE serve para músicas, pilha e fila, e comparações ou liberações específicas são feitas por callbacks (por exemplo, `musica_destruir` em `lse_destruir`).
- Quem é dono das cargas? A biblioteca é dona das músicas: ela as cria com `musica_criar` e as libera com `musica_destruir`. A lista é dona só dos nós; `lse_remover` devolve a carga e libera apenas o nó. `lse_destruir(lista, NULL)` libera só os nós, e com callback não nulo libera também cada carga.
- Como tratar referências compartilhadas? Histórico e fila guardam apenas ponteiros para músicas da biblioteca, sem copiá-las. Por isso, antes de liberar uma música, é preciso remover as referências a ela de fila e histórico, para não deixar ponteiros inválidos.
- Quando adicionar ponteiro `fim`? (A decidir na M08, quando a fila for implementada.) Sem `fim`, inserir no final percorre a lista toda (O(n)). A fila precisa de inserção no final e remoção no início em O(1), então o `fim` passa a valer a pena nessa etapa, mantendo a invariante de que `fim` aponta para o último nó e é `NULL` com a lista vazia.

## M02 — Decisões sobre a LSE
- `t_elem` e os campos de `t_lse` ficam definidos apenas em `lse.c`; `lse.h` expõe só o tipo opaco `t_lse`. Assim, pilha e fila usam a lista pela interface pública e não dependem da representação interna.
- A lista guarda `void *carga` e não conhece `t_musica`, o que a mantém genérica.
- `lse_destruir` libera sempre os nós e a lista; as cargas só são liberadas quando um callback destruidor é passado. Isso evita liberar duas vezes músicas que pertencem à biblioteca.
- O ponteiro `fim` continua adiado (ver decisão da M08).


## M03 — Encadeamento e percurso
- **Desenho do encadeamento** (inserindo `a`, depois `b`, depois `c` no início):
  ```
  vazia:     inicio -> NULL                          tamanho = 0
  após a:    inicio -> [a|*] -> NULL                 tamanho = 1
  após b:    inicio -> [b|*] -> [a|*] -> NULL        tamanho = 2
  após c:    inicio -> [c|*] -> [b|*] -> [a|*] -> NULL   tamanho = 3
  ```
  Inserir no início muda 2 ponteiros: `novo->prox = inicio` e depois `inicio = novo`. A ordem importa: se `inicio` fosse atualizado primeiro, o antigo primeiro seria perdido (vazamento).
- `lse_percorrer` usa um callback `void (*)(void *carga)`. A aplicação recebe só a carga, nunca o `t_elem`; por isso `t_elem` continua privado a `lse.c`.
- `criar_no` é uma função `static` de `lse.c`: centraliza o `malloc` do nó e é reaproveitada por `lse_inserir_fim`.
- Responsabilidades de memória: **a lista é dona dos nós; quem insere é dono da carga**, exceto quando `lse_destruir` recebe um callback destruidor.
- Decisão sobre carga `NULL`: aceita na M03 (ver `docs/contratos.md`).

## M04 — Inserção no fim, busca e remoção
- **Remoção (três casos)**, com `anterior` e `atual` percorrendo a cadeia:
  ```
  cabeça:  inicio = atual->prox
  meio:    anterior->prox = atual->prox
  cauda:   anterior->prox = NULL   (mesmo código do meio, pois atual->prox == NULL)
  único:   inicio = NULL, tamanho = 0
  ```
  Em todos os casos o nó é liberado **depois** de religar a cadeia e de guardar a carga em uma variável local.
- `lse_buscar` e `lse_remover` recebem `t_comparador`: `lse.c` não contém código específico de `t_musica`. O teste de músicas compara por `id` fora da lista.
- Custos: `lse_inserir_fim` é O(n) (sem ponteiro `fim`; a decisão de adicioná-lo segue para a M08); busca e remoção por chave são O(n); `lse_remover_inicio` e `lse_inserir_inicio` são O(1).
- Quem libera o quê: `lse_remover`/`lse_remover_inicio` liberam só o nó e **devolvem a carga**. A biblioteca (dona das músicas) é quem chama `musica_destruir` depois de retirar as referências de fila e histórico.
- Ambiguidade de `NULL`: ver `docs/contratos.md`.
