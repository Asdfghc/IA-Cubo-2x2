#include <iostream>
#include <array>
#include <queue>
#include <set>
#include <algorithm>

struct Estado {
    std::array<int,7> pos; // cubie em cada posição global 0..6 (sem o 7 = DRB)
    std::array<int,7> ori; // orientação (0,1,2)

    void print() const {
        for(int i=0; i<7; i++) {
            std::cout << "pos[" << i << "] = " << pos[i] << ", ori[" << i << "] = " << ori[i] << "\n";
        }
    }

    bool operator==(const Estado& rhs) const {
        return pos == rhs.pos && ori == rhs.ori;
    }

    bool operator<(const Estado& rhs) const {
        return pos != rhs.pos || ori != rhs.ori;
    }
};

struct No {
    Estado estado;
    char movimento;
    No *pai;

    void print() const {
        estado.print();
        std::cout << "movimento = " << movimento << "\n";
    }
};

Estado aplicarMovimento(const Estado& s, char mov) {
    Estado r = s;

    switch(mov) {
        case 'U': // U face horário: 0->3->2->1->0
            r.pos[0] = s.pos[3];
            r.pos[1] = s.pos[0];
            r.pos[2] = s.pos[1];
            r.pos[3] = s.pos[2];

            r.ori[0] = s.ori[3];
            r.ori[1] = s.ori[0];
            r.ori[2] = s.ori[1];
            r.ori[3] = s.ori[2];
            break;

        case 'u': // U inverso
            r.pos[0] = s.pos[1];
            r.pos[1] = s.pos[2];
            r.pos[2] = s.pos[3];
            r.pos[3] = s.pos[0];

            r.ori[0] = s.ori[1];
            r.ori[1] = s.ori[2];
            r.ori[2] = s.ori[3];
            r.ori[3] = s.ori[0];
            break;

        case 'L': // L face horário: 1->2->6->5->1
            r.pos[1] = s.pos[2];
            r.pos[2] = s.pos[6];
            r.pos[6] = s.pos[5];
            r.pos[5] = s.pos[1];

            r.ori[1] = (s.ori[2] + 1) % 3;
            r.ori[2] = (s.ori[6] + 2) % 3;
            r.ori[6] = (s.ori[5] + 1) % 3;
            r.ori[5] = (s.ori[1] + 2) % 3;
            break;

        case 'l': // L inverso
            r.pos[1] = s.pos[5];
            r.pos[2] = s.pos[1];
            r.pos[6] = s.pos[2];
            r.pos[5] = s.pos[6];

            r.ori[1] = (s.ori[5] + 1) % 3;
            r.ori[2] = (s.ori[1] + 2) % 3;
            r.ori[6] = (s.ori[2] + 1) % 3;
            r.ori[5] = (s.ori[6] + 2) % 3;
            break;

        case 'F': // F face horário: 0->1->5->4->0
            r.pos[0] = s.pos[1];
            r.pos[1] = s.pos[5];
            r.pos[5] = s.pos[4];
            r.pos[4] = s.pos[0];

            r.ori[0] = (s.ori[1] + 1) % 3;
            r.ori[1] = (s.ori[5] + 2) % 3;
            r.ori[5] = (s.ori[4] + 1) % 3;
            r.ori[4] = (s.ori[0] + 2) % 3;
            break;

        case 'f': // F inverso
            r.pos[0] = s.pos[4];
            r.pos[1] = s.pos[0];
            r.pos[5] = s.pos[1];
            r.pos[4] = s.pos[5];

            r.ori[0] = (s.ori[4] + 1) % 3;
            r.ori[1] = (s.ori[0] + 2) % 3;
            r.ori[5] = (s.ori[1] + 1) % 3;
            r.ori[4] = (s.ori[5] + 2) % 3;
            break;

        default:
            // movimento inválido, mantém o estado
            break;
    }

    return r;
}


int main() {
    Estado estado_inicial = {{2,3,1,4,0,5,6}, {1,2,0,0,2,0,1}};
    Estado final = {{0,1,2,3,4,5,6}, {0,0,0,0,0,0,0}};

    std::queue<No*> Estrutura;
    std::set<Estado> Explorados;
    std::vector<No*> todos_nos; // para liberar memória depois

    No* inicial = new No{estado_inicial, '\0', nullptr};
    Estrutura.push(inicial);
    todos_nos.push_back(inicial);

    bool achou = false;
    while (!Estrutura.empty()) {
        No* atual = Estrutura.front();
        Estrutura.pop();

        if (atual->estado == final) {
            std::cout << "achou" << std::endl;
            // Reconstruir caminho
            std::vector<No*> caminho;
            No* ponteiro = atual;
            while (ponteiro != nullptr) {
                caminho.push_back(ponteiro);
                ponteiro = ponteiro->pai;
            }
            std::reverse(caminho.begin(), caminho.end());
            for (size_t i = 1; i < caminho.size(); ++i) { // começa do 1 para ignorar o nó inicial
                std::cout << caminho[i]->movimento << std::endl;
                //caminho[i]->estado.print();
            }
            achou = true;
            break;
        }

        Explorados.insert(atual->estado);

        // Gerar próximos estados
        struct Mov {
            char c;
            Estado e;
        } movs[] = {
            {'U', aplicarMovimento(atual->estado, 'U')},
            {'u', aplicarMovimento(atual->estado, 'u')},
            {'L', aplicarMovimento(atual->estado, 'L')},
            {'l', aplicarMovimento(atual->estado, 'l')},
            {'F', aplicarMovimento(atual->estado, 'F')},
            {'f', aplicarMovimento(atual->estado, 'f')}
        };
        for (const auto& mov : movs) {
            if (Explorados.find(mov.e) == Explorados.end()) {
                No* novo = new No{mov.e, mov.c, atual};
                Estrutura.push(novo);
                todos_nos.push_back(novo);
            }
        }
    }

    if (!achou) {
        std::cout << "nao achou";
    }

    // Libera memória
    for (No* n : todos_nos) delete n;
    return 0;
}
