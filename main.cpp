#include <iostream>
#include <array>

struct Estado {
    std::array<int,7> pos; // cubie em cada posição global 0..6 (sem o 7 = DRB)
    std::array<int,7> ori; // orientação (0,1,2)

    void print() const {
        for(int i=0; i<7; i++) {
            std::cout << "pos[" << i << "] = " << pos[i] << ", ori[" << i << "] = " << ori[i] << "\n";
        }
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
    Estado s = {{0,1,2,3,4,5,6},
            {0,0,0,0,0,0,0}};

    std::cout << "Estado inicial:\n";
    s.print();

    
    Estado r = aplicarMovimento(s, 'L');
    std::cout << "\nApós L:\n";
    r.print();

    Estado u = aplicarMovimento(r, 'U');
    std::cout << "\nApós U:\n";
    u.print();

    Estado f = aplicarMovimento(u, 'f');
    std::cout << "\nApós F':\n";
    f.print();
    

    //Estado a = aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(s, 'U'), 'l'), 'U'), 'l'), 'U'), 'l'), 'U'), 'l'), 'U');
    //Estado b = aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(aplicarMovimento(a, 'l'), 'U'), 'l'), 'U'), 'l'), 'U'), 'l'), 'U'), 'l');

    //b.print();

    return 0;
}
