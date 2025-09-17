#include <array>
#include <queue>
#include <fstream>
#include <iostream>
#include "cube_state.h"

using namespace std;

static uint8_t dist[N_ORI * N_PERM]; // distâncias até o resolvido

int main() {
    carregarTabelas();

    // Inicializa todas as distâncias com 0xFF (não visitado)
    fill(begin(dist), end(dist), 0xFF);

    queue<EstadoCodificado> q;

    // Começa do estado resolvido
    uint32_t coord = EstadoCodificado::packState(resolvido.oriCoord, resolvido.permCoord);
    dist[coord] = 0;
    q.push(resolvido);

    size_t visitados = 1;
    while (!q.empty()) {
        EstadoCodificado atual = q.front();
        q.pop();
        uint8_t distancia = dist[EstadoCodificado::packState(atual.oriCoord, atual.permCoord)];

        for (int m = 0; m < N_MOV; m++) {
            EstadoCodificado prox = EstadoCodificado::aplicarMovimento(atual, movimentos[m]);
            coord = EstadoCodificado::packState(prox.oriCoord, prox.permCoord);
            if (dist[coord] == 0xFF) {
                dist[coord] = distancia + 1;
                q.push(prox);
                visitados++;
            }
        }
    }

    // Salva a heurística em arquivo
    ofstream f("tables/heuristic.bin", ios::binary);
    f.write((char*)dist, sizeof(dist));
    f.close();

    cout << "Heurística gerada. Estados visitados = " << visitados << endl;
}
