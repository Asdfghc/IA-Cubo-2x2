# IA-Cubo-2x2
Este projeto implementa um solucionador para o Cubo Mágico 2x2 utilizando técnicas de busca em grafos, como BFS, DFS e A*.

Para isso, foram utilizados **C++** para a implementação dos algoritmos e estruturas de dados e **OpenGL** para a visualização do cubo e das soluções encontradas.

TODO: (Imagem)

## Algoritmos implementados:
- ### BFS - Busca em largura com otimização para evitar estados repetidos.

- ### DFS - Busca em profundidade limitada ao máximo teórico.

- ### A* - Busca informada com otimização para evitar estados repetidos e com heurística baseada na distância até o estado resolvido, pré-computada.
É possível adicionar ruído à heurística para simular e analisar o algoritmo A* em diferentes cenários.

# Como usar:
### Requisitos:
- **g++** (para compilar o código)
- **OpenGL** e **GLUT** (para a visualização gráfica)

## Automaticamente:

```
./run.sh
```

## Manualmente:

### 1. Precomputação da tabela de movimentos:
- **Compilar:** `g++ precompute/precompute_moves.cpp src/cube_state.cpp -Iinclude -o build/precompute_moves`

- **Rodar:** `./build/precompute_moves`

### 2. Precomputação da heurística:
- **Compilar:** `g++ precompute/precompute_heuristic.cpp src/cube_state.cpp -Iinclude -o build/precompute_heuristic`

- **Rodar:** `./build/precompute_heuristic`

### 3. Principal
- **Compilar:** `g++ src/*.cpp -Iinclude -o build/main -lGL -lGLU -lglut`

- **Rodar:** `./build/main`

#### (Opcional) Para teste standalone do solver:
- **Compilar:** `g++ src/solver.cpp src/cube_state.cpp -DSOLVER_STANDALONE -Iinclude -o build/solver`

- **Rodar:** `./build/solver`

# Melhorias futuras:
- Interface gráfica mais amigável, com animações e mais movimentos.

- Implementação de mais algoritmos de busca como BFS bidirecional, IDA* e two-phase algorithm.

- Testar e comparar diferentes pesos para a heurística no A*.

# Testes: 
A média ponderada foi calculada com base na chance de um estado com cada distância (quantidade de movimentos mínima até resolver) ser gerado aleatoriamente, considerando o número de estados possíveis para cada distância.

#### BFS
|           | Média ponderada |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|-----------|-----------------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|
| Distância  | 10.67           | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 |
| Quantidade de estados | - | 6 | 27 | 120 | 534 | 2,256 | 8,969 | 33,058 | 114,149 | 360,508 | 930,588 | 1,350,852 | 782,536 | 90,280 | 276 |
| Tempo médio (s) | 4.171 | 0.000 | 0.000 | 0.000 | 0.001 | 0.004 | 0.014 | 0.054 | 0.274 | 0.910 | 2.587 | 5.314 | 6.541 | 6.859 | 8.082 |
| Estados visitados | 2,803,270.18 | 18.250 | 93.600 | 414.250 | 1,566.900 | 7,438.850 | 28,367.650 | 99,598.300 | 361,923.150 | 950,024.850 | 2,289,552.300 | 3,399,188.750 | 3,651,140.200 | 3,674,096.900 | 3,674,160.000 |

#### DFS
|           | Média ponderada |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|-----------|-----------------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|
| Distância  | 10.67           | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 |
| Quantidade de estados | - | 6 | 27 | 120 | 534 | 2,256 | 8,969 | 33,058 | 114,149 | 360,508 | 930,588 | 1,350,852 | 782,536 | 90,280 | 276 |
| Tempo médio (s) | 5.760 | 0.014 | 0.012 | 0.042 | 0.037 | 0.506 | 0.396 | 1.442 | 1.291 | 4.693 | 1.894 | 10.344 | 2.750 | 15.360 | 5.213 |
| Estados visitados | 17,494,353.24 | 49,393.800 | 41,722.200 | 140,280.050 | 115,859.100 | 1,705,282.700 | 1,323,572.750 | 4,750,615.400 | 3,989,098.250 | 14,810,336.800 | 5,879,749.350 | 30,933,288.600 | 9,036,481.250 | 44,040,582.900 | 15,652,357.450 |

#### A*
|           | Média ponderada |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
|-----------|-----------------|---|---|---|---|---|---|---|---|---|----|----|----|----|----|
| Distância  | 10.67           | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 |
| Quantidade de estados | - | 6 | 27 | 120 | 534 | 2,256 | 8,969 | 33,058 | 114,149 | 360,508 | 930,588 | 1,350,852 | 782,536 | 90,280 | 276 |
| Tempo médio (s) | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.000 | 0.001 |
| Estados visitados | 37.08 | 2.000 | 3.250 | 4.300 | 5.200 | 6.850 | 8.350 | 9.350 | 11.700 | 15.750 | 21.550 | 34.750 | 67.500 | 99.250 | 132.100 |
