##  Desafio de um processo seletivo para vaga de desenvolvedor back-end
Implementar uma biblioteca que processe arquivos CSV, aplicando filtros e selecionando colunas conforme especificado. A solução deve ser capaz de integrar com a interface definida em C abaixo.

```c
/**
 * Process the CSV data by applying filters and selecting columns.
 *
 * @param csv The CSV data to be processed.
 * @param selectedColumns The columns to be selected from the CSV data.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void processCsv(const char[], const char[], const char[]);

/**
 * Process the CSV file by applying filters and selecting columns.
 *
 * @param csvFilePath The path to the CSV file to be processed.
 * @param selectedColumns The columns to be selected from the CSV data.
 * @param rowFilterDefinitions The filters to be applied to the CSV data.
 *
 * @return void
 */
void processCsvFile(const char[], const char[], const char[]);
```
## Funcionalidades
-   [x] **A primeira linha do CSV sempre será um header**
-   [x] **Filtros para maior (>), menor (<) e igual (=)**
-   [x] **Uma string de seleção de colunas vazia é equivalente a selecionar todas as colunas**
-   [x] **Os comparadores nos filtros seguem a ordem lexicográfica**
-   [x] **As colunas que aparecem na string de colunas selecionadas podem estar em ordem arbitrária**
-   [x] **As colunas na string de filtros podem aparecer em ordem arbitrária**
-   [ ] **Colunas que não existem podem aparecer na seleção de colunas e nos filtros**
-   [ ] **Tratamento de erro para filtros inválidos**
-   [ ] **Aceitar mais de 1 filtro por header**
-   [ ] **Implementar os operadores diferente (!=), Maior ou igual que (>=), e Menor ou igual que (<=)**




