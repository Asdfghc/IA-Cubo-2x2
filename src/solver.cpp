#include <iostream>
#include <chrono>
#include <array>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <unordered_set>
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
    vector<No*> todos_nos;

    No* inicial = new No{estado_inicial, Movimento::None, nullptr};
    Estrutura.push(inicial);
    todos_nos.push_back(inicial);
    visitados.insert(packState(estado_inicial.oriCoord, estado_inicial.permCoord)); // Marca inicial como visitado

    bool achou = false;
    while (!Estrutura.empty()) {
        No* atual = Estrutura.front();
        Estrutura.pop();

        // TODO: Melhorar print de progresso
        /*if (Estrutura.size() % 1000 == 1) {
            cout << "Estados na fila: " << Estrutura.size() << ", visitados: " << visitados.size() << "\r";
            cout.flush();
        }*/

        if (atual->estado == final) {
            // Reconstruir caminho
            vector<No*> caminho_nos;
            No* ponteiro = atual;
            while (ponteiro != nullptr) {
                caminho_nos.push_back(ponteiro);
                ponteiro = ponteiro->pai;
            }
            reverse(caminho_nos.begin(), caminho_nos.end());
            for (size_t i = 1; i < caminho_nos.size(); ++i) { // começa do 1 para ignorar o nó inicial
                caminho.push_back(caminho_nos[i]->movimento);
            }
            achou = true;

            float duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - now).count();
            cout << "Tempo de execução: " << duration/1000 << " segundos" << endl;
            cout << "Nº de estados explorados: " << visitados.size() << endl;
            break;
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
            No* novo = new No{proximo, movimento, atual, static_cast<uint8_t>(atual->profundidade + 1)};
            Estrutura.push(novo);
            todos_nos.push_back(novo);
        }
    }

    // Libera memória
    for (No* n : todos_nos) delete n;
    return achou;
}

// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_dfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho) {
    int visitados = 0;
    auto now = chrono::system_clock::now();

    stack<No*> Estrutura;
    vector<No*> todos_nos;

    No* inicial = new No{estado_inicial, Movimento::None, nullptr};
    Estrutura.push(inicial);
    todos_nos.push_back(inicial);

    bool achou = false;
    while (!Estrutura.empty()) {
        No* atual = Estrutura.top();
        Estrutura.pop();

        if (atual->estado == final) {
            // Reconstruir caminho
            vector<No*> caminho_nos;
            No* ponteiro = atual;
            while (ponteiro != nullptr) {
                caminho_nos.push_back(ponteiro);
                ponteiro = ponteiro->pai;
            }
            reverse(caminho_nos.begin(), caminho_nos.end());
            for (size_t i = 1; i < caminho_nos.size(); ++i) { // começa do 1 para ignorar o nó inicial
                caminho.push_back(caminho_nos[i]->movimento);
            }
            achou = true;

            float duration = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - now).count();
            cout << "Tempo de execução: " << duration/1000 << " segundos" << endl;
            cout << "Nº de estados explorados: " << visitados << endl;
            break; // TODO: Buscar mais de uma solução
            
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
            
            uint32_t key = packState(proximo.oriCoord, proximo.permCoord);

            visitados++;
            No* novo = new No{proximo, movimento, atual, static_cast<uint8_t>(atual->profundidade + 1)};
            Estrutura.push(novo);
            todos_nos.push_back(novo);
        }
    }

    // Libera memória
    for (No* n : todos_nos) delete n;
    return achou;
}

#ifdef SOLVER_STANDALONE
// main para teste standalone
int main() {
    carregarTabelas();
    
    EstadoCodificado estado_inicial = {412, 224};
    vector<Movimento> caminho;
    
    bool achou = solve_bfs(estado_inicial, caminho);
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
