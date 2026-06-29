# Sistema de Consultas com Tabela Hash e Filtro de Bloom

## Disciplina

**Laboratório de Estrutura de Dados II**

## Integrantes

* José Eduardo Freitas Oliveira
* Reubismar Gomes dos Santos Junior
* Ruan Dantas Bandeira

---

# Descrição do Projeto

Este projeto tem como objetivo implementar e integrar duas importantes estruturas de dados: uma **Tabela Hash** e um **Filtro de Bloom**, permitindo realizar operações de inserção e consulta de usuários de forma eficiente.

A Tabela Hash é responsável pelo armazenamento efetivo dos dados, enquanto o Filtro de Bloom atua como uma estrutura probabilística de pré-verificação, reduzindo consultas desnecessárias à tabela principal.

O sistema foi desenvolvido para analisar o comportamento dessas estruturas em diferentes cenários de utilização, observando métricas como quantidade de colisões, consultas evitadas, falsos positivos e tempo médio de consulta.

---

# Objetivos

* Implementar uma Tabela Hash utilizando encadeamento separado para tratamento de colisões;
* Implementar um Filtro de Bloom utilizando vetor de bits;
* Integrar as duas estruturas em um único sistema;
* Comparar consultas realizadas com e sem a utilização do Filtro de Bloom;
* Avaliar o impacto dos falsos positivos no desempenho da aplicação;
* Coletar estatísticas para análise dos resultados.

---

# Estrutura do Projeto

```text
projeto/
│
├── src/
│   ├── hash.c
│   ├── hash.h
│   ├── bloom.c
│   ├── bloom.h
│   └── main.c
│
├── data/
│   ├── consulta_1000.txt
│   ├── consulta_10000.txt
│   ├── consulta_100000.txt
│   ├── usuario_1000.txt
│   ├── usuario_10000.txt
│   └── usuario_100000.txt
│
├── test/
│
├── .gitignore
├── main.exe
├── README.md
└── relatorio.pdf
```

---

# Funcionalidades Implementadas

## Inserção de Usuários

Permite inserir usuários individualmente no sistema.

Durante a inserção:

1. O usuário é armazenado na Tabela Hash;
2. O mesmo usuário é registrado no Filtro de Bloom;
3. As estatísticas do sistema são atualizadas.

---

## Consulta de Usuários

O sistema oferece dois modos de consulta.

### Consulta sem Filtro de Bloom

A busca é realizada diretamente na Tabela Hash.

### Consulta com Filtro de Bloom

O fluxo de consulta segue o seguinte processo:

```text
Usuário
   ↓
Filtro de Bloom
   ↓
Tabela Hash
```

Caso o filtro indique que o elemento não existe, a consulta à Tabela Hash é evitada.

---

## Inserção em Lote

O sistema permite inserir automaticamente usuários armazenados em arquivos de texto.

Essa funcionalidade foi utilizada para realização dos experimentos e coleta de estatísticas.

---

## Estatísticas

O sistema apresenta as seguintes métricas:

* Quantidade de elementos armazenados;
* Quantidade de consultas realizadas;
* Quantidade de consultas evitadas;
* Número de falsos positivos;
* Taxa de falsos positivos;
* Quantidade de colisões;
* Tempo médio de consulta.

---

# Estruturas Utilizadas

## Tabela Hash

A Tabela Hash utiliza:

* Encadeamento separado;
* Alocação dinâmica de memória;
* Função Hash baseada no algoritmo DJB2;
* Busca e inserção em tempo médio O(1).

### Tratamento de Colisões

Quando dois elementos produzem o mesmo índice na tabela, é utilizada uma lista encadeada para armazená-los na mesma posição.

---

## Filtro de Bloom

O Filtro de Bloom utiliza:

* Vetor de bits;
* Operações bit a bit (Bitwise);
* Múltiplas funções hash simuladas por sementes;
* Dimensionamento automático do vetor e da quantidade de funções hash.

### Características

O Filtro de Bloom pode produzir:

* Verdadeiros Positivos;
* Verdadeiros Negativos;
* Falsos Positivos.

Entretanto, nunca produz falsos negativos.

---

# Fórmulas Utilizadas

## Dimensionamento do Vetor de Bits

```text
m = -(n × ln(p)) / (ln(2)²)
```

Onde:

* n = quantidade esperada de elementos;
* p = probabilidade desejada de falso positivo.

---

## Quantidade de Funções Hash

```text
k = (m / n) × ln(2)
```

Onde:

* m = tamanho do vetor de bits;
* n = quantidade esperada de elementos.

---

# Compilação

Para compilar o projeto em ambiente Windows utilizando GCC:

```bash
gcc .\src\main.c .\src\hash.c .\src\bloom.c -o main
```

---

# Execução

Após a compilação:

```bash
.\main.exe
```

---

# Fluxo Geral do Sistema

```text
Início
   ↓
Escolha do experimento
   ↓
Criação da Tabela Hash
   ↓
Criação do Filtro de Bloom
   ↓
Menu Principal
   ├── Inserir Usuário
   ├── Consultar Usuário
   ├── Inserção em Lote
   ├── Estatísticas
   └── Sair
   ↓
Liberação da Memória
   ↓
Fim
```

---

# Resultados Esperados

Com a utilização do Filtro de Bloom espera-se:

* Redução significativa das consultas realizadas na Tabela Hash;
* Baixa taxa de falsos positivos;
* Melhor desempenho em cenários com grande volume de dados;
* Redução do custo computacional das operações de busca.

---

# Considerações Finais

Este projeto demonstra a aplicação prática de estruturas de dados amplamente utilizadas em sistemas modernos de armazenamento e recuperação de informações. A integração entre a Tabela Hash e o Filtro de Bloom permite explorar conceitos de eficiência, otimização de consultas e análise de desempenho, fornecendo uma solução capaz de lidar com grandes quantidades de dados de forma eficiente.

---

# Licença

Projeto desenvolvido exclusivamente para fins acadêmicos na disciplina de Laboratório de Estrutura de Dados II.
