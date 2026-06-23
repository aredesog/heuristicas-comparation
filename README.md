# Comparação de Heurísticas para Problemas NP-Difíceis

Este projeto tem como objetivo implementar e comparar duas abordagens heurísticas para resolução de problemas de otimização em grafos:

* **Busca Local**
* **Algoritmo Evolutivo**

O foco do trabalho é analisar o desempenho dessas técnicas considerando a qualidade das soluções encontradas e o tempo de execução.

## Integrantes

* Guilherme Aredes
* Carlos Eduardo
* Erick Pereira
* Marcos Felipe
* Diogo Silva

---

## Objetivo

Problemas NP-Difíceis frequentemente possuem espaços de busca muito grandes, tornando inviável a obtenção da solução ótima por métodos exatos em instâncias de grande porte.

Dessa forma, este trabalho compara duas metaheurísticas amplamente utilizadas:

1. **Busca Local**, baseada em melhorias sucessivas de uma solução inicial.
2. **Algoritmo Evolutivo**, inspirado em mecanismos de seleção natural e evolução.

Os algoritmos são avaliados considerando:

* Qualidade da solução obtida;
* Tempo de execução;
* Escalabilidade em instâncias de diferentes tamanhos.

---

## Estrutura do Projeto

```text
.
.├── tp-4/
.│ └── instances/
.│ └── instâncias de teste
.├── .gitignore
.├── Makefile
.├── README.md
.├── base.c
.├── gera.c
.├── heuristicas.c
.└── heuristicas.h
```

---

## Representação da Instância

As instâncias são representadas por um grafo ponderado não direcionado.

Formato do arquivo:

```text
<num_nos> <num_arestas>
<u> <v> <peso>
<u> <v> <peso>
...
```

Exemplo:

```text
4 6
0 1 10
0 2 15
0 3 20
1 2 35
1 3 25
2 3 30
```

---

## Algoritmos Implementados

### Busca Local

A busca local parte de uma solução inicial e explora soluções vizinhas em busca de melhorias sucessivas.

Características:

* Construção de solução inicial;
* Exploração de vizinhança;
* Aceitação de melhorias;
* Critério de parada baseado em estagnação ou limite de iterações.

### Algoritmo Evolutivo

O algoritmo evolutivo trabalha com uma população de soluções candidatas.

Etapas:

* Inicialização da população;
* Avaliação de fitness;
* Seleção dos indivíduos;
* Operadores de crossover;
* Operadores de mutação;
* Critério de parada baseado em número de gerações.

---

Exemplo de compilação:

```bash
gcc gerador.c -o gerador
```

Execução:

```bash
./gerador > instancia.dat
```

---

## Compilação

Utilizando Makefile:

```bash
make
```

Ou manualmente:

```bash
gcc src/*.c -Iinclude -o heuristicas
```

---

## Execução

Executando uma instância:

```bash
./heuristicas instancia.dat
```

Saída esperada:

```text
Busca Local: <valor_solucao> <tempo_execucao>
Evolutivo: <valor_solucao> <tempo_execucao>
```

Onde:

* O primeiro valor corresponde à qualidade da solução;
* O segundo valor representa o tempo de execução em segundos.

---
# Valores

As heurísticas são comparadas utilizando:

* Valor da solução obtida;
* Tempo de execução;
* Comportamento em diferentes tamanhos de instância.

