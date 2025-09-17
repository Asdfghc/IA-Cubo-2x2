#include <array>
#include <queue>
#include <fstream>
#include <iostream>
#include "cube_state.h"

constexpr int TOTAL_STATES = N_ORI * N_PERM;
static uint8_t dist[TOTAL_STATES]; // distância até o resolvido

int main() {
    carregarTabelas();

    // inicializa todas as distâncias com 0xFF (não visitado)
    std::fill(std::begin(dist), std::end(dist), 0xFF);

    std::queue<EstadoCodificado> q;

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

    std::ofstream f("tables/heuristic.bin", std::ios::binary);
    f.write((char*)dist, sizeof(dist));
    f.close();

    std::cout << "Heurística gerada. Estados visitados = " << visitados << std::endl;
}
