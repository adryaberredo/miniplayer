# MiniPlayer — ICC002 (2026/2)
Projeto integrador progressivo, em duplas, em C. Reprodução **simulada**.

## Compilação
`make` e `./build/miniplayer`.

## Testes
`make test`. **ATENÇÃO:** os testes estão preparados para falhar até que os stubs sejam implementados. Isso é intencional. Comece com `make build/test_musica && ./build/test_musica`.

## Estado atual
- **M01 concluída:** `musica_criar` e `musica_destruir` implementadas em `src/musica.c`; `test_musica` passa.
- **M02, M03 e M04 concluídas:** `lse_criar/tamanho/primeiro/destruir`, `lse_inserir_inicio`, `lse_percorrer`, `lse_inserir_fim`, `lse_buscar`, `lse_remover` e `lse_remover_inicio` implementadas em `src/lse.c`. Rode `make test_missoes` (ou `make test_m02`, `make test_m03`, `make test_m04`); `test_lse` também passa.
- `lse_inserir_ordenado` (M05), `test_pilha` e `test_fila` seguem falhando de propósito até as missões correspondentes.
- Como executar (a partir da pasta `miniplayer/`): `make build/test_musica && ./build/test_musica`.

## Contratos
- `void *carga` é genérico; a lista não conhece `t_musica`.
- A lista possui os nós, **não** as cargas, salvo quando `lse_destruir` recebe callback destruidor.
- Remover devolve a carga e libera apenas o nó.
- Pilha e fila reutilizam `lse_*`, sem manipular `t_elem`.
- Biblioteca possui as músicas; histórico e fila só as referenciam.
- Antes de liberar uma música, remover referências a ela de fila e histórico.

## Como documentar
Preencher `docs/arquitetura.md`, `docs/contratos.md`, `docs/diario-de-bordo.md` e `docs/relatorio-testes.md` a cada missão. Ver `GUIA_ESTUDANTE.md`.
