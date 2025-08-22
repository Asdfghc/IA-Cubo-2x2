Instalar as dependências (linux): `sudo apt-get install libglfw3-dev libglew-dev libglm-dev`
Compilar: `g++ src/*.cpp solver/solver.cpp -Iinclude -lglfw -lGLEW -lGL -o build/main`
Rodar: `./build/main`


Para teste do solver:
Compilar: `g++ solver/solver.cpp -Iinclude -o build/solver`
Rodar: `./build/solver`