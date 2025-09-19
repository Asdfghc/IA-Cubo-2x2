#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <array>
#include <fstream>
#include <numeric>
#include <functional>
#include <iomanip>
#include "cube_state.h"
#include "solver.h"

using namespace std;

constexpr int MAX_DIST = 14;
constexpr int N_TESTES_POR_DIST = 20; // Ajuste conforme desejado

// Gera um estado aleatório de distância exata 'dist' usando a tabela de heurística
EstadoCodificado gerar_estado_distancia(int dist, mt19937& rng, const std::vector<uint32_t>& indices) {
    uniform_int_distribution<size_t> dist_idx(0, indices.size() - 1);
    uint32_t packed = indices[dist_idx(rng)];
    uint16_t ori = packed / N_PERM;
    uint16_t perm = packed % N_PERM;
    return EstadoCodificado{ori, perm};
}

struct Resultado {
    double tempo_ms = 0;
    double visitados = 0;
};

void testar_algoritmo(const string& nome, function<bool(const EstadoCodificado&, vector<Movimento>&, int&)> solver_func) {
    mt19937 rng(42); // Semente fixa para reprodutibilidade
    array<Resultado, MAX_DIST+1> resultados{};

    // Carrega a tabela de heurística
    std::vector<uint8_t> heuristic(N_ORI * N_PERM);
    std::ifstream f("tables/heuristic.bin", ios::binary);
    if (!f) {
        cerr << "Não foi possível abrir tables/heuristic.bin" << endl;
        return;
    }
    f.read(reinterpret_cast<char*>(heuristic.data()), heuristic.size());
    f.close();

    // Pré-seleciona os índices de cada distância
    std::vector<std::vector<uint32_t>> indices_por_dist(MAX_DIST+1);
    for (uint32_t i = 0; i < heuristic.size(); ++i) {
        uint8_t d = heuristic[i];
        if (d >= 1 && d <= MAX_DIST) {
            indices_por_dist[d].push_back(i);
        }
    }

    for (int dist = 1; dist <= MAX_DIST; ++dist) {
        if (indices_por_dist[dist].empty()) {
            cout << nome << " dist=" << dist << " não há estados disponíveis." << endl;
            resultados[dist].tempo_ms = -1;
            resultados[dist].visitados = -1;
            continue;
        }
        double soma_tempo = 0;
        double soma_visitados = 0;
        int resolvidos = 0;
        for (int t = 0; t < N_TESTES_POR_DIST; ++t) {
            EstadoCodificado estado = gerar_estado_distancia(dist, rng, indices_por_dist[dist]);
            vector<Movimento> caminho;
            int n_visitados = 0;
            auto ini = chrono::high_resolution_clock::now();
            bool achou = solver_func(estado, caminho, n_visitados);
            auto fim = chrono::high_resolution_clock::now();
            double tempo = chrono::duration<double, milli>(fim - ini).count();
            //if (achou && caminho.size() == dist) {
                soma_tempo += tempo;
                soma_visitados += n_visitados;
                resolvidos++;
            //}
            if ((t+1) % (N_TESTES_POR_DIST/5) == 0) {
                cout << nome << " dist=" << dist << " teste " << (t+1) << "/" << N_TESTES_POR_DIST << "\r" << flush;
            }
        }
        cout << nome << " dist=" << dist << " resolvidos: " << resolvidos << "/" << N_TESTES_POR_DIST << endl;
        if (resolvidos > 0) {
            resultados[dist].tempo_ms = soma_tempo / resolvidos;
            resultados[dist].visitados = soma_visitados / resolvidos;
        } else {
            resultados[dist].tempo_ms = -1;
            resultados[dist].visitados = -1;
        }
    }
    // Imprime resultados em formato de tabela
    cout << fixed << setprecision(3);
    cout << "\n" << nome << " Tempo médio (s): ";
    for (int d = 1; d <= MAX_DIST; ++d) cout << (resultados[d].tempo_ms / 1000.0) << " ";
    cout << "\n" << nome << " Estados visitados: ";
    for (int d = 1; d <= MAX_DIST; ++d) cout << resultados[d].visitados << " ";
    cout << endl;
}

int main() {
    carregarTabelas();
    cout << "Testando BFS..." << endl;
    testar_algoritmo("BFS", solve_bfs);
    cout << "Testando DFS..." << endl;
    testar_algoritmo("DFS", solve_dfs);
    cout << "Testando A*..." << endl;
    auto astar_func = [](const EstadoCodificado& e, vector<Movimento>& c, int& n) { return solve_astar(e, c, n, 0); };
    testar_algoritmo("A*", astar_func);
    return 0;
}

/*
Tabelas carregadas!
Testando BFS...
BFS dist=1 resolvidos: 20/20
BFS dist=2 resolvidos: 20/20
BFS dist=3 resolvidos: 20/20
BFS dist=4 resolvidos: 20/20
BFS dist=5 resolvidos: 20/20
BFS dist=6 resolvidos: 20/20
BFS dist=7 resolvidos: 20/20
BFS dist=8 resolvidos: 20/20
BFS dist=9 resolvidos: 20/20
BFS dist=10 resolvidos: 20/20
BFS dist=11 resolvidos: 20/20
BFS dist=12 resolvidos: 20/20
BFS dist=13 resolvidos: 20/20
BFS dist=14 resolvidos: 20/20

BFS Tempo médio (s): 0.000 0.000 0.000 0.001 0.004 0.014 0.054 0.274 0.910 2.587 5.314 6.541 6.859 8.082 
BFS Estados visitados: 18.250 93.600 414.250 1566.900 7438.850 28367.650 99598.300 361923.150 950024.850 2289552.300 3399188.750 3651140.200 3674096.900 3674160.000 
Testando DFS...
DFS dist=1 resolvidos: 20/20
DFS dist=2 resolvidos: 20/20
DFS dist=3 resolvidos: 20/20
DFS dist=4 resolvidos: 20/20
DFS dist=5 resolvidos: 20/20
DFS dist=6 resolvidos: 20/20
DFS dist=7 resolvidos: 20/20
DFS dist=8 resolvidos: 20/20
DFS dist=9 resolvidos: 20/20
DFS dist=10 resolvidos: 20/20
DFS dist=11 resolvidos: 20/20
DFS dist=12 resolvidos: 20/20
DFS dist=13 resolvidos: 20/20
DFS dist=14 resolvidos: 20/20

DFS Tempo médio (s): 0.014 0.012 0.042 0.037 0.506 0.396 1.442 1.291 4.693 1.894 10.344 2.750 15.360 5.213 
DFS Estados visitados: 49393.800 41722.200 140280.050 115859.100 1705282.700 1323572.750 4750615.400 3989098.250 14810336.800 5879749.350 30933288.600 9036481.250 44040582.900 15652357.450 
Testando A*...
A* dist=1 resolvidos: 20/20
A* dist=2 resolvidos: 20/20
A* dist=3 resolvidos: 20/20
A* dist=4 resolvidos: 20/20
A* dist=5 resolvidos: 20/20
A* dist=6 resolvidos: 20/20
A* dist=7 resolvidos: 20/20
A* dist=8 resolvidos: 20/20
A* dist=9 resolvidos: 20/20
A* dist=10 resolvidos: 20/20
A* dist=11 resolvidos: 20/20
A* dist=12 resolvidos: 20/20
A* dist=13 resolvidos: 20/20
A* dist=14 resolvidos: 20/20

A* Tempo médio (s): 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.000 0.001 
A* Estados visitados: 2.000 3.250 4.300 5.200 6.850 8.350 9.350 11.700 15.750 21.550 34.750 67.500 99.250 132.100
*/