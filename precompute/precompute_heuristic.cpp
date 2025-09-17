#include <array>
#include <queue>
#include <fstream>
#include <iostream>
#include "cube_state.h"

using namespace std;

static uint8_t dist[N_ORI * N_PERM]; // distância até o resolvido

int main() {
    carregarTabelas();

    // inicializa todas as distâncias com 0xFF (não visitado)
    fill(begin(dist), end(dist), 0xFF);

    queue<EstadoCodificado> q;

    EstadoCodificado resolvido = {0, 0}; // cubo resolvido
    uint32_t id = EstadoCodificado::packState(resolvido.oriCoord, resolvido.permCoord);
    dist[id] = 0;
    q.push(resolvido);

    size_t visitados = 1;
    while (!q.empty()) {
        EstadoCodificado atual = q.front();
        q.pop();
        uint8_t d = dist[EstadoCodificado::packState(atual.oriCoord, atual.permCoord)];

        for (int m = 0; m < N_MOV; m++) {
            EstadoCodificado prox = EstadoCodificado::aplicarMovimento(atual, movimentos[m]);
            uint32_t idp = EstadoCodificado::packState(prox.oriCoord, prox.permCoord);
            if (dist[idp] == 0xFF) {
                dist[idp] = d + 1;
                q.push(prox);
                visitados++;
            }
        }
    }

    ofstream f("tables/heuristic.bin", ios::binary);
    f.write((char*)dist, sizeof(dist));
    f.close();

    cout << "Heurística gerada. Estados visitados = " << visitados << endl;
}
