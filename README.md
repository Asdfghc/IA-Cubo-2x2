# Principal
Compilar: `g++ src/*.cpp -Iinclude -o build/main -lGL -lGLU -lglut`\
Rodar: `./build/main`  
# Para teste do solver:
Compilar: `g++ src/solver.cpp src/cube_state.cpp -DSOLVER_STANDALONE -Iinclude -o build/solver`\
Rodar: `./build/solver`  
# Precomputação da tabela de movimentos:
Compilar: `g++ precompute/precompute_moves.cpp src/cube_state.cpp -Iinclude -o build/precompute_moves`\
Rodar: `./build/precompute_moves`  
# Precomputação da heurística:
Compilar: `g++ precompute/precompute_heuristic.cpp src/cube_state.cpp -Iinclude -o build/precompute_heuristic`\
Rodar: `./build/precompute_heuristic`  
<br/>
<br/>
# TODOs:
- pesos na a*?
- bilateral bfs?
- two-fase algorithm??
---
- ui
- escolha de algoritmo
- animações?
<br/>
<br/>
# Testes:

### Com/sem set de explorados:

### Com/sem eliminação simples de movimentos:

### Com/sem otimização dos estados:


# Algoritmos:

### BFS:

### DFS:

### IDA*:

### A* com heurística com ruído: