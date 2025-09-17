#include "cube_state.h"
#include <fstream>
#include <iostream>

using namespace std;

// Definição das tabelas
uint16_t oriMove[N_ORI][N_MOV];
uint16_t permMove[N_PERM][N_MOV];
uint8_t heuristic[N_ORI * N_PERM];

// Carrega as tabelas de arquivos binários
void carregarTabelas() {
    ifstream f1("tables/oriMove.bin", ios::binary);
    f1.read((char*)oriMove, sizeof(oriMove));
    f1.close();

    ifstream f2("tables/permMove.bin", ios::binary);
    f2.read((char*)permMove, sizeof(permMove));
    f2.close();

    ifstream f3("tables/heuristic.bin", ios::binary);
    f3.read((char*)heuristic, sizeof(heuristic));
    f3.close();

    cout << "Tabelas carregadas!" << endl;
}
