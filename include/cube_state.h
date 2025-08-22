#pragma once

#include <array>
#include <iostream>
#include <map>

const char movimentos[] = {'U', 'u', 'L', 'l', 'F', 'f'};
inline char movimentos_inversos(char mov) {
    switch(mov) {
        case 'U': return 'u';
        case 'u': return 'U';
        case 'L': return 'l';
        case 'l': return 'L';
        case 'F': return 'f';
        case 'f': return 'F';
        default: return mov;
    }
}

struct Estado {
    std::array<uint8_t,7> pos; // cubie em cada posição global 0..6 (sem o 7 = DRB)
    std::array<uint8_t,7> ori; // orientação (0,1,2)

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

    static Estado aplicarMovimento(const Estado& s, char mov) {
        Estado r = s;

        switch(mov) {
            case 'U': // U
                r.pos[0] = s.pos[3];
                r.pos[1] = s.pos[0];
                r.pos[2] = s.pos[1];
                r.pos[3] = s.pos[2];

                r.ori[0] = s.ori[3];
                r.ori[1] = s.ori[0];
                r.ori[2] = s.ori[1];
                r.ori[3] = s.ori[2];
                break;

            case 'u': // U'
                r.pos[0] = s.pos[1];
                r.pos[1] = s.pos[2];
                r.pos[2] = s.pos[3];
                r.pos[3] = s.pos[0];

                r.ori[0] = s.ori[1];
                r.ori[1] = s.ori[2];
                r.ori[2] = s.ori[3];
                r.ori[3] = s.ori[0];
                break;

            case 'L': // L
                r.pos[1] = s.pos[2];
                r.pos[2] = s.pos[6];
                r.pos[6] = s.pos[5];
                r.pos[5] = s.pos[1];

                r.ori[1] = (s.ori[2] + 1) % 3;
                r.ori[2] = (s.ori[6] + 2) % 3;
                r.ori[6] = (s.ori[5] + 1) % 3;
                r.ori[5] = (s.ori[1] + 2) % 3;
                break;

            case 'l': // L'
                r.pos[1] = s.pos[5];
                r.pos[2] = s.pos[1];
                r.pos[6] = s.pos[2];
                r.pos[5] = s.pos[6];

                r.ori[1] = (s.ori[5] + 1) % 3;
                r.ori[2] = (s.ori[1] + 2) % 3;
                r.ori[6] = (s.ori[2] + 1) % 3;
                r.ori[5] = (s.ori[6] + 2) % 3;
                break;

            case 'F': // F
                r.pos[0] = s.pos[1];
                r.pos[1] = s.pos[5];
                r.pos[5] = s.pos[4];
                r.pos[4] = s.pos[0];

                r.ori[0] = (s.ori[1] + 1) % 3;
                r.ori[1] = (s.ori[5] + 2) % 3;
                r.ori[5] = (s.ori[4] + 1) % 3;
                r.ori[4] = (s.ori[0] + 2) % 3;
                break;

            case 'f': // F'
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
};