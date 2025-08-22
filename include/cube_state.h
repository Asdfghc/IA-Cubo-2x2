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


// Estado empacotado em um único inteiro (até 32 bits)
struct Estado {
    // 7 posições (0..6) cada uma cabe em 3 bits: 21 bits
    // 7 orientações (0..2) cada uma cabe em 2 bits: 14 bits
    // Total: 35 bits, mas como não há permutação repetida, pode-se usar 32 bits para aplicações práticas
    // Aqui, para simplicidade, usamos dois campos, mas ambos são compactados
    uint32_t packed_pos; // 21 bits usados
    uint16_t packed_ori; // 14 bits usados

    Estado() : packed_pos(0), packed_ori(0) {}

    Estado(const std::array<uint8_t,7>& pos, const std::array<uint8_t,7>& ori) : packed_pos(0), packed_ori(0) {
        for (int i = 0; i < 7; ++i) {
            packed_pos |= (pos[i] & 0x7) << (3*i);
            packed_ori |= (ori[i] & 0x3) << (2*i);
        }
    }

    std::array<uint8_t,7> get_pos() const {
        std::array<uint8_t,7> pos{};
        for (int i = 0; i < 7; ++i) {
            pos[i] = (packed_pos >> (3*i)) & 0x7;
        }
        return pos;
    }

    std::array<uint8_t,7> get_ori() const {
        std::array<uint8_t,7> ori{};
        for (int i = 0; i < 7; ++i) {
            ori[i] = (packed_ori >> (2*i)) & 0x3;
        }
        return ori;
    }

    inline void print() const {
        auto pos = get_pos();
        auto ori = get_ori();
        for(int i=0; i<7; i++) {
            std::cout << "pos[" << i << "] = " << int(pos[i]) << ", ori[" << i << "] = " << int(ori[i]) << "\n";
        }
    }

    inline bool operator==(const Estado& rhs) const {
        return packed_pos == rhs.packed_pos && packed_ori == rhs.packed_ori;
    }

    inline bool operator<(const Estado& rhs) const {
        return packed_pos < rhs.packed_pos || (packed_pos == rhs.packed_pos && packed_ori < rhs.packed_ori);
    }

    inline static Estado aplicarMovimento(const Estado& s, char mov) {
        auto pos = s.get_pos();
        auto ori = s.get_ori();

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
            }
            default:
                break;
        }
        return Estado(pos, ori);
    }
};