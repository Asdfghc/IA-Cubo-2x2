#include <iostream>
#include <chrono>
#include <array>
#include <queue>
#include <set>
#include <algorithm>
#include "cube_state.h"

using namespace std;

struct No {
    Estado estado;
    char movimento;
    No *pai;

    void print() const {
        estado.print();
        cout << "movimento = " << movimento << "\n";
    }
};

const Estado final = {{0,1,2,3,4,5,6}, {0,0,0,0,0,0,0}};
// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_bfs(const Estado& estado_inicial, vector<char>& caminho) {
    int explorados = 0;
    auto now = chrono::system_clock::now();

    queue<No*> Estrutura;
    vector<No*> todos_nos; // para liberar memória depois

    No* inicial = new No{estado_inicial, '\0', nullptr};
    Estrutura.push(inicial);
    todos_nos.push_back(inicial);

    bool achou = false;
    while (!Estrutura.empty()) {
        No* atual = Estrutura.front();
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
            cout << "Nº de estados explorados: " << explorados << endl;
            break;
            
        }

        explorados++;

        // Gerar próximos estados
        for (char movimento : movimentos) {
            // Não permite movimento inverso do anterior
            if (atual->movimento != '\0' && movimento == movimentos_inversos(atual->movimento))
                continue;
            // Não permite 3 movimentos iguais seguidos
            if (atual->pai && atual->movimento == movimento && atual->pai->movimento == movimento)
                continue;
            if (atual->pai && atual->movimento == movimento && (movimento == 'u' || movimento == 'l' || movimento == 'f'))
                continue;
            
            Estado proximo = Estado::aplicarMovimento(atual->estado, movimento);
            No* novo = new No{proximo, movimento, atual};
            Estrutura.push(novo);
            todos_nos.push_back(novo);
        }
    }

    // Libera memória
    cout << "Total de memória alocada: " << todos_nos.size() * sizeof(No) << " bytes" << endl;
    for (No* n : todos_nos) delete n;
    return achou;
}

// main para teste standalone
int main() {
    Estado estado_inicial = {{2,3,1,4,0,5,6}, {1,2,0,0,2,0,1}};
    vector<char> caminho;
    
    bool achou = solve_bfs(estado_inicial, caminho);
    if (achou) {
        cout << "achou: ";
        for (char mov : caminho) {
            cout << mov << " ";
        }
    } else {
        cout << "nao achou";
    }
    cout << endl;
    return 0;
}
