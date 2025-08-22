#include <iostream>
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
bool solve(const Estado& estado_inicial, vector<char>& caminho) {

    queue<No*> Estrutura;
    set<Estado> Explorados;
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
            //cout << "Nº de estados explorados: " << Explorados.size() << endl;
            break;
        }

        Explorados.insert(atual->estado);

        // Gerar próximos estados
        for (char movimento : movimentos) {
            Estado proximo = Estado::aplicarMovimento(atual->estado, movimento);
            if (Explorados.find(proximo) == Explorados.end()) {
                No* novo = new No{proximo, movimento, atual};
                Estrutura.push(novo);
                todos_nos.push_back(novo);
            }
        }
    }

    // Libera memória
    for (No* n : todos_nos) delete n;
    return achou;
}

// main para teste standalone
int main() {
    Estado estado_inicial = {{2,3,1,4,0,5,6}, {1,2,0,0,2,0,1}};
    vector<char> caminho;
    bool achou = solve(estado_inicial, caminho);
    if (achou) {
        cout << "achou" << endl;
        for (char mov : caminho) {
            cout << mov << endl;
        }
    } else {
        cout << "nao achou";
    }
    return 0;
}
