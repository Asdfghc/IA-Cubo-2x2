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

### 1. Precomputação da tabela de movimentos:
Compilar: `g++ precompute/precompute_moves.cpp src/cube_state.cpp -Iinclude -o build/precompute_moves`\
Rodar: `./build/precompute_moves`  

### 2. Precomputação da heurística:
Compilar: `g++ precompute/precompute_heuristic.cpp src/cube_state.cpp -Iinclude -o build/precompute_heuristic`\
Rodar: `./build/precompute_heuristic`  

### 3. Principal
Compilar: `g++ src/*.cpp -Iinclude -o build/main -lGL -lGLU -lglut`\
Rodar: `./build/main`

#### (Opcional) Para teste standalone do solver:
Compilar: `g++ src/solver.cpp src/cube_state.cpp -DSOLVER_STANDALONE -Iinclude -o build/solver`\
Rodar: `./build/solver`

# Melhorias futuras:
- Interface gráfica mais amigável, com animações e mais movimentos.

- Implementação de mais algoritmos de busca como BFS bidirecional, IDA* e two-phase algorithm.

# Testes: 
TODO:

### BFS sem/com set de explorados:
- #### Sem set de explorados:
  - Tempo médio: 
  - Nº médio de estados visitados: 
- #### Com set de explorados:
  - Tempo médio: 
  - Nº médio de estados visitados: 

### Sem/Com eliminação simples de movimentos:

- #### BFS:
  - #### Sem eliminação simples de movimentos:
    - Tempo médio: 
    - Nº médio de estados visitados: 
  - #### Com eliminação simples de movimentos:
    - Tempo médio: 
    - Nº médio de estados visitados: 

- #### DFS:
    - #### Sem eliminação simples de movimentos:
        - Tempo médio: 
        - Nº médio de estados visitados: 
    - #### Com eliminação simples de movimentos:
        - Tempo médio: 
        - Nº médio de estados visitados: 


### Sem/com pesagem na A*:
- #### Sem pesagem:
  - Tempo médio: 
  - Nº médio de estados visitados: 
- #### Com pesagem:
  - Tempo médio: 
  - Nº médio de estados visitados:
