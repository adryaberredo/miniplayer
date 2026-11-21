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

## Casos-limite
- Estrutura vazia: não se aplica à M01 (ainda não há lista). `musica_destruir(NULL)` é seguro (T07).
- Estrutura com um elemento: uma única música criada e destruída sem vazamento (T01, T08).
- Chave inexistente: não se aplica à M01 (busca só a partir da M04). Entradas inválidas (título/artista NULL, duração negativa) devolvem NULL (T02 a T04).
- Falha de alocação (discutir estratégia de simulação): se `malloc` devolver NULL, `musica_criar` retorna NULL e não altera nada. Para simular, pode-se compilar um teste com um `malloc` substituto que sempre devolve NULL, ou envolver a alocação em uma função que o teste consiga forçar a falhar.

## Memória
- Compilação: `make build/test_musica && ./build/test_musica` (gcc, `-std=c11 -Wall -Wextra -Wpedantic -g`).
- Verificação extra: compilado com `-fsanitize=address,undefined` para T02 a T08, sem erros nem vazamentos.
- Problemas corrigidos: aviso `-Wnewline-eof` (faltava quebra de linha no fim de `src/musica.c`); cópia de strings feita com `strncpy` mais terminador '\0' explícito para evitar estouro do buffer.