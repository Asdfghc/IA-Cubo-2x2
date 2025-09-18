#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

mkdir -p build
mkdir -p tables
OUTPUT="build/main"

check_tables() {
    if [ -z "$(ls -A tables)" ]; then
        return 1
    else
        return 0
    fi
}

# Função para compilar e rodar pré-computação
precompute_tables() {
    echo -e "${YELLOW}Tabelas não encontradas. Iniciando pré-computação...${NC}"

    echo -e "${GREEN}Compilando precompute_moves...${NC}"
    g++ precompute/precompute_moves.cpp src/cube_state.cpp -Iinclude -o build/precompute_moves
    if [ $? -ne 0 ]; then
        echo -e "${RED}Erro ao compilar precompute_moves.${NC}"
        exit 1
    fi

    echo -e "${GREEN}Executando precompute_moves...${NC}"
    ./build/precompute_moves
    if [ $? -ne 0 ]; then
        echo -e "${RED}Erro ao executar precompute_moves.${NC}"
        exit 1
    fi

    echo -e "${GREEN}Compilando precompute_heuristic...${NC}"
    g++ precompute/precompute_heuristic.cpp src/cube_state.cpp -Iinclude -o build/precompute_heuristic
    if [ $? -ne 0 ]; then
        echo -e "${RED}Erro ao compilar precompute_heuristic.${NC}"
        exit 1
    fi

    echo -e "${GREEN}Executando precompute_heuristic...${NC}"
    ./build/precompute_heuristic
    if [ $? -ne 0 ]; then
        echo -e "${RED}Erro ao executar precompute_heuristic.${NC}"
        exit 1
    fi

    echo -e "${GREEN}Pré-computação concluída com sucesso!${NC}"
}

# Verifica se precisa rodar pré-computação
check_tables
if [ $? -ne 0 ]; then
    precompute_tables
fi

echo -e "${GREEN}Compilando programa principal...${NC}"
ERRORS=$(g++ src/*.cpp -Iinclude -o "$OUTPUT" -lGL -lGLU -lglut 2>&1)

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Compilação bem-sucedida!${NC}"
    echo -e "${GREEN}Executando programa principal.${NC}"
    ./"$OUTPUT"
else
    echo -e "${RED}Erro na compilação.${NC}"
    echo -e "${RED}${ERRORS}${NC}"

    echo "$ERRORS" > build_errors.log
    echo -e "${RED}Erros salvos em build_errors.log${NC}"
    exit 1
fi
