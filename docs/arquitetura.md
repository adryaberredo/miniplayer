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