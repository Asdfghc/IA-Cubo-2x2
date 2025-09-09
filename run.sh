#!/bin/bash

# Cores para saida colorida
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

mkdir -p build
OUTPUT="build/main"

echo -e "${GREEN}Compilando...${NC}"
ERRORS=$(g++ src/*.cpp -Iinclude -o "$OUTPUT" -lGL -lGLU -lglut 2>&1)

if [ $? -eq 0 ]; then
	echo -e "${GREEN}Compilação bem-sucedida!${NC}"
	echo -e "${GREEN}Executando.${NC}"
	./"$OUTPUT"
else
	echo -e "${RED}Erro na compilação.${NC}"
	echo -e "${RED}${ERRORS}${NC}"

	echo "$ERRORS" > build_errors.log
	echo -e "${RED}Erros salvos em build_errors.log${NC}"
	exit 1
fi
