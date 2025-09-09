#pragma once
#include <array>
#include <cstdint>

// --- Constantes de movimentos ---

constexpr char movimentos[] = {'U', 'u', 'L', 'l', 'F', 'f'};
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

// --- Estrutura do estado do cubo ---

struct Estado {
    std::array<uint8_t,7> pos; // cubie em cada posição global 0..6 (sem o 7 = DRB)
    std::array<uint8_t,7> ori; // orientação (0,1,2)

    bool operator==(const Estado& rhs) const {
        return pos == rhs.pos && ori == rhs.ori;
    }

    bool operator<(const Estado& rhs) const {
        return pos != rhs.pos || ori != rhs.ori;
    }

    // Aplica um movimento ao estado e retorna o novo estado
    static Estado aplicarMovimento(const Estado& s, char mov) {
        Estado r = s;

        switch(mov) {
            case 'U': {
                uint8_t tmp = pos[0];
                pos[0] = pos[3];
                pos[3] = pos[2];
                pos[2] = pos[1];
                pos[1] = tmp;
                tmp = ori[0];
                ori[0] = ori[3];
                ori[3] = ori[2];
                ori[2] = ori[1];
                ori[1] = tmp;
                break;
            }
            case 'u': {
                uint8_t tmp = pos[0];
                pos[0] = pos[1];
                pos[1] = pos[2];
                pos[2] = pos[3];
                pos[3] = tmp;
                tmp = ori[0];
                ori[0] = ori[1];
                ori[1] = ori[2];
                ori[2] = ori[3];
                ori[3] = tmp;
                break;
            }
            case 'L': {
                uint8_t tmp = pos[1];
                pos[1] = pos[2];
                pos[2] = pos[6];
                pos[6] = pos[5];
                pos[5] = tmp;
                uint8_t t1 = ori[1], t2 = ori[2], t3 = ori[6], t4 = ori[5];
                ori[1] = (t2 + 1) % 3;
                ori[2] = (t3 + 2) % 3;
                ori[6] = (t4 + 1) % 3;
                ori[5] = (t1 + 2) % 3;
                break;
            }
            case 'l': {
                uint8_t tmp = pos[1];
                pos[1] = pos[5];
                pos[5] = pos[6];
                pos[6] = pos[2];
                pos[2] = tmp;
                uint8_t t1 = ori[1], t2 = ori[5], t3 = ori[6], t4 = ori[2];
                ori[1] = (t2 + 1) % 3;
                ori[5] = (t3 + 2) % 3;
                ori[6] = (t4 + 1) % 3;
                ori[2] = (t1 + 2) % 3;
                break;
            }
            case 'F': {
                uint8_t tmp = pos[0];
                pos[0] = pos[1];
                pos[1] = pos[5];
                pos[5] = pos[4];
                pos[4] = tmp;
                uint8_t t1 = ori[0], t2 = ori[1], t3 = ori[5], t4 = ori[4];
                ori[0] = (t2 + 1) % 3;
                ori[1] = (t3 + 2) % 3;
                ori[5] = (t4 + 1) % 3;
                ori[4] = (t1 + 2) % 3;
                break;
            }
            case 'f': {
                uint8_t tmp = pos[0];
                pos[0] = pos[4];
                pos[4] = pos[5];
                pos[5] = pos[1];
                pos[1] = tmp;
                uint8_t t1 = ori[0], t2 = ori[4], t3 = ori[5], t4 = ori[1];
                ori[0] = (t2 + 1) % 3;
                ori[4] = (t3 + 2) % 3;
                ori[5] = (t4 + 1) % 3;
                ori[1] = (t1 + 2) % 3;
                break;
            default:
                break;
        }
        return r;
    }
};
