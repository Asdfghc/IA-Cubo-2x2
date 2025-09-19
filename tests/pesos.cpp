#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <array>

using namespace std;

constexpr int N_ORI = 729;
constexpr int N_PERM = 5040;
constexpr int MAX_DIST = 14; // profundidade máxima esperada

int main() {
    array<uint8_t, N_ORI * N_PERM> heuristic;
    ifstream f("tables/heuristic.bin", ios::binary);
    if (!f) {
        cerr << "Não foi possível abrir tables/heuristic.bin" << endl;
        return 1;
    }
    f.read(reinterpret_cast<char*>(heuristic.data()), heuristic.size());
    f.close();

    array<size_t, MAX_DIST + 2> contagem{}; // +1 para 0, +1 para não visitados

    for (size_t i = 0; i < heuristic.size(); ++i) {
        uint8_t d = heuristic[i];
        if (d == 0xFF) {
            contagem[MAX_DIST + 1]++; // não visitado
        } else if (d <= MAX_DIST) {
            contagem[d]++;
        } else {
            // distâncias maiores que MAX_DIST
            contagem[MAX_DIST]++;
        }
    }

    cout << "Distância\tQuantidade" << endl;
    for (int d = 0; d <= MAX_DIST; ++d) {
        cout << d << "\t" << contagem[d] << endl;
    }
    cout << ">" << MAX_DIST << "\t" << contagem[MAX_DIST] << endl;
    cout << "Não visitado\t" << contagem[MAX_DIST + 1] << endl;
    return 0;
}

/*
Distância       Quantidade
0       1
1       6
2       27
3       120
4       534
5       2256
6       8969
7       33058
8       114149
9       360508
10      930588
11      1350852
12      782536
13      90280
14      276
*/