#include "cube_state.h"
#include <fstream>
#include <iostream>

// Definição das tabelas
uint16_t oriMove[N_ORI][N_MOV];
uint16_t permMove[N_PERM][N_MOV];
uint8_t heuristic[N_ORI * N_PERM];

// Funções
void carregarTabelas() {
    std::ifstream f1("tables/oriMove.bin", std::ios::binary);
    f1.read((char*)oriMove, sizeof(oriMove));
    f1.close();

    std::ifstream f2("tables/permMove.bin", std::ios::binary);
    f2.read((char*)permMove, sizeof(permMove));
    f2.close();

    std::ifstream f3("tables/heuristic.bin", std::ios::binary);
    f3.read((char*)heuristic, sizeof(heuristic));
    f3.close();

    std::cout << "Tabelas carregadas!" << std::endl;
}

uint32_t packState(uint16_t ori, uint16_t perm) {
    return (static_cast<uint32_t>(ori) * N_PERM) + perm;
}