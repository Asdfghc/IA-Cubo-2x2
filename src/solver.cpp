
#include <iostream>
#include <chrono>
#include <array>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <unordered_set>
#include <memory>
#include "cube_state.h"

using namespace std;

struct No {
    EstadoCodificado estado;
    Movimento movimento;
    No *pai;
    uint8_t profundidade;
};

const EstadoCodificado final = {0, 0};
// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_bfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho) {
    unordered_set<uint32_t> visitados;
    auto now = chrono::system_clock::now();

    queue<No*> Estrutura;
    vector<unique_ptr<No>> pool;

    auto inicial = make_unique<No>(No{estado_inicial, Movimento::None, nullptr});
    Estrutura.push(inicial.get());
    pool.push_back(move(inicial));
    visitados.insert(packState(estado_inicial.oriCoord, estado_inicial.permCoord)); // Marca inicial como visitado

    while (!Estrutura.empty()) {
        No* atual = Estrutura.front();
        Estrutura.pop();

        // TODO: Melhorar print de progresso e colocar nos outros
        /*if (Estrutura.size() % 1000 == 1) {
            cout << "Estados na fila: " << Estrutura.size() << ", visitados: " << visitados.size() << "\r";
            cout.flush();
        }*/

        if (atual->estado == final) {
            // Reconstruir caminho
            for (No* n = atual; n->pai != nullptr; n = n->pai) {
                caminho.push_back(n->movimento);
            }
            reverse(caminho.begin(), caminho.end());

            float duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - now).count();
            cout << "Tempo de execução: " << duration/1000 << " segundos" << endl;
            cout << "Nº de estados explorados: " << visitados.size() << endl;
            
            return true;
        }

        // Limita profundidade
        if (atual->profundidade >= 14) {
            continue;
        }

        // Gerar próximos estados
        for (Movimento movimento : movimentos) {
            // Não permite movimento inverso do anterior
            if (movimento == movimentos_inversos(atual->movimento))
                continue;
            // Não permite 3 movimentos iguais seguidos
            if (atual->pai && atual->movimento == movimento && atual->pai->movimento == movimento)
                continue;
            // Não permite dois movimentos anti-horários iguais seguidos
            if (atual->movimento == movimento && (movimento == Movimento::u || movimento == Movimento::l || movimento == Movimento::f))
                continue;

            EstadoCodificado proximo = EstadoCodificado::aplicarMovimento(atual->estado, movimento);
            
            uint32_t key = packState(proximo.oriCoord, proximo.permCoord);

            if (visitados.count(key)) continue;

            visitados.insert(key);
            auto novo = make_unique<No>(No{proximo, movimento, atual, static_cast<uint8_t>(atual->profundidade + 1)});
            Estrutura.push(novo.get());
            pool.push_back(move(novo));
        }
    }

    return false;
}

// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_dfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho) {
    int visitados = 0;
    auto now = chrono::system_clock::now();

    stack<No*> Estrutura;
    vector<unique_ptr<No>> pool;

    auto inicial = make_unique<No>(No{estado_inicial, Movimento::None, nullptr});
    Estrutura.push(inicial.get());
    pool.push_back(move(inicial));

    bool achou = false;
    while (!Estrutura.empty()) {
        No* atual = Estrutura.top();
        Estrutura.pop();

        if (atual->estado == final) {
            // Reconstruir caminho
            for (No* n = atual; n->pai != nullptr; n = n->pai) {
                caminho.push_back(n->movimento);
            }
            reverse(caminho.begin(), caminho.end());

            float duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - now).count();
            cout << "Tempo de execução: " << duration/1000 << " segundos" << endl;
            cout << "Nº de estados explorados: " << visitados << endl;

            return true; // TODO: Buscar mais de uma solução
        }

        // Limita profundidade
        if (atual->profundidade >= 14) {
            continue;
        }

        // TODO: Liberar memória de nós não mais necessários

        // Gerar próximos estados
        for (Movimento movimento : movimentos) {
            // Não permite movimento inverso do anterior
            if (atual->pai && movimento == movimentos_inversos(atual->movimento))
                continue;
            // Não permite 3 movimentos iguais seguidos
            if (atual->pai && atual->movimento == movimento && atual->pai->movimento == movimento)
                continue;
            // Não permite dois movimentos anti-horários iguais seguidos
            if (atual->movimento == movimento && (movimento == Movimento::u || movimento == Movimento::l || movimento == Movimento::f))
                continue;
            
            EstadoCodificado proximo = EstadoCodificado::aplicarMovimento(atual->estado, movimento);

            visitados++;
            auto novo = make_unique<No>(No{proximo, movimento, atual, static_cast<uint8_t>(atual->profundidade + 1)});
            Estrutura.push(novo.get());
            pool.push_back(move(novo));
        }
    }

    return false;
}

// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_astar(const EstadoCodificado& inicial, vector<Movimento>& caminho, int ruido) {
    auto now = chrono::system_clock::now();

    auto cmp = [&](No* a, No* b) {
        uint32_t id_a = packState(a->estado.oriCoord, a->estado.permCoord);
        uint32_t id_b = packState(b->estado.oriCoord, b->estado.permCoord);

        uint8_t h_a = heuristic[id_a];
        uint8_t h_b = heuristic[id_b];

            // Adiciona ruído simétrico entre -ruido e +ruido
            if (ruido > 0) {
                srand(time(nullptr));
                h_a += (rand() % (2 * ruido + 1)) - ruido;
                h_b += (rand() % (2 * ruido + 1)) - ruido;
        }

        int f_a = a->profundidade + h_a; // TODO: pesar heurística
        int f_b = b->profundidade + 1 + h_b;

        return f_a > f_b; // menor f tem prioridade
    };

    priority_queue<No*, vector<No*>, decltype(cmp)> pq(cmp);

    unordered_set<uint32_t> visitados;
    vector<unique_ptr<No>> pool;

    auto root = make_unique<No>(No{inicial, Movimento::None, nullptr, 0});
    pq.push(root.get());
    pool.push_back(move(root));

    while (!pq.empty()) {
        No* atual = pq.top();
        pq.pop();

        uint32_t id = packState(atual->estado.oriCoord, atual->estado.permCoord);
        if (visitados.count(id)) continue;
        visitados.insert(id);

        if (atual->estado == final) {
            // reconstrói o caminho
            for (No* n = atual; n->pai != nullptr; n = n->pai) {
                caminho.push_back(n->movimento);
            }
            reverse(caminho.begin(), caminho.end());

            float duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - now).count();
            cout << "Tempo de execução: " << duration/1000 << " segundos" << endl;
            cout << "Nº de estados explorados: " << visitados.size() << endl;

            return true;
        }

        for (int m = 0; m < N_MOV; m++) {
            EstadoCodificado prox = EstadoCodificado::aplicarMovimento(atual->estado, movimentos[m]);
            auto filho = make_unique<No>(No{prox, movimentos[m], atual, (uint8_t)(atual->profundidade + 1)});
            pq.push(filho.get());
            pool.push_back(move(filho));
        }
    }

    return false;
}  

#ifdef SOLVER_STANDALONE
// main para teste standalone
int main() {
    carregarTabelas();
    
    EstadoCodificado estado_inicial = {412, 224};
    vector<Movimento> caminho;
    
    bool achou = solve_astar(estado_inicial, caminho, 0);
    if (achou) {
        cout << "achou: ";
        for (Movimento mov : caminho) {
            cout << movimento_to_char(mov) << " ";
        }
    } else {
        cout << "nao achou";
    }
    cout << endl;
    
    return 0;
}
#endif
