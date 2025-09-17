#include <array>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include "cube_state.h"

using namespace std;

// --- Pré-computação das move tables ---
int main() {
    // Orientações
    for (int i = 0; i < N_ORI; i++) {
        EstadoDecodificado s;
        s.pos = {0,1,2,3,4,5,6,7};
        s.ori = EstadoCodificado::coordToOri(i);

        for (int m = 0; m < N_MOV; m++) {
            auto r = EstadoDecodificado::aplicarMovimento(s, movimentos[m]);
            oriMove[i][m] = EstadoCodificado::oriToCoord(r.ori);
        }
    }

    // Permutações
    for (int i = 0; i < N_PERM; i++) {
        EstadoDecodificado s;
        s.pos = EstadoCodificado::coordToPerm(i);
        s.ori = {0,0,0,0,0,0,0,0};

        for (int m = 0; m < N_MOV; m++) {
            auto r = EstadoDecodificado::aplicarMovimento(s, movimentos[m]);
            permMove[i][m] = EstadoCodificado::permToCoord(r.pos);
        }
    }

    // Salva as tabelas em arquivos binários
    ofstream f1("tables/oriMove.bin", ios::binary);
    f1.write((char*)oriMove, sizeof(oriMove));
    f1.close();
    
    ofstream f2("tables/permMove.bin", ios::binary);
    f2.write((char*)permMove, sizeof(permMove));
    f2.close();

    cout << "Tabelas geradas e salvas!" << endl;
}
