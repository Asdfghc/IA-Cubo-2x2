Instalar as dependências (linux): `sudo apt-get install libglfw3-dev libglew-dev libglm-dev`
Compilar: `g++ src/*.cpp solver/solver.cpp -Iinclude -lglfw -lGLEW -lGL -o build/main`
Rodar: `./build/main`


Para teste do solver:
Compilar: `g++ solver/solver.cpp -Iinclude -o build/solver`
Rodar: `./build/solver`


# Testes:
### Com set de explorados:
Tempo de execução: 2.606 segundos
Nº de estados explorados: 573751
achou: u f L u L L F U 

# Sem set de explorados:
Tempo de execução: 0.564 segundos
Nº de estados explorados: 866515
achou: u f L u L L F U 

# Com eliminação de movimentos repetidos
Tempo de execução: 0.201 segundos
Nº de estados explorados: 217943
Total de memória alocada: 75767400 bytes
achou: u f L u L L F U 

# Com otimização dos estados
Tempo de execução: 0.108 segundos
Nº de estados explorados: 217943
Total de memória alocada: 25255800 bytes
achou: u f L u L L F U 

# Com mais eliminação de movimentos repetidos
Tempo de execução: 0.055 segundos
Nº de estados explorados: 127405
Total de memória alocada: 13605360 bytes
achou: u f L u L L F U 