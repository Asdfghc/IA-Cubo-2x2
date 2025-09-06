# Principal
Compilar: `g++ src/*.cpp -Iinclude -o build/main -lGL -lGLU -lglut`\
Rodar: `./build/main`
# Para teste do solver:
Compilar: `g++ src/solver.cpp -DSOLVER_STANDALONE -Iinclude -o build/solver`\
Rodar: `./build/solver`
<br/>
<br/>
# TODOs:
- melhorar dfs
- a* com heurística perfeita com ruído
- ida* com LUT completa?
- set de explorados (hash)?
- bilateral bfs?
- two-fase alghorithm?
---
- ui
- animações?
<br/>
<br/>
# Testes:

### Com set de explorados (sem hash):
Tempo de execução: 2.606 segundos  
Nº de estados explorados: 573751  
achou: u f L u L L F U  

### Sem set de explorados:
Tempo de execução: 0.564 segundos  
Nº de estados explorados: 866515  
achou: u f L u L L F U  

### Com eliminação de movimentos repetidos
Tempo de execução: 0.201 segundos  
Nº de estados explorados: 217943  
Total de memória alocada: 75767400 bytes  
achou: u f L u L L F U  

### Com otimização dos estados
Tempo de execução: 0.108 segundos  
Nº de estados explorados: 217943  
Total de memória alocada: 25255800 bytes  
achou: u f L u L L F U  

### Com mais eliminação de movimentos repetidos
Tempo de execução: 0.055 segundos  
Nº de estados explorados: 127405  
Total de memória alocada: 13605360 bytes  
achou: u f L u L L F U  