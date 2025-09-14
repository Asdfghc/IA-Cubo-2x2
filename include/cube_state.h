#pragma once
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

// --- Constantes de movimentos ---

enum class Movimento : unsigned char {
    U=0, u=1, L=2, l=3, F=4, f=5, None=6
};

constexpr Movimento movimentos[] = {Movimento::U, Movimento::u, Movimento::L, Movimento::l, Movimento::F, Movimento::f};
inline Movimento movimentos_inversos(Movimento mov) {
    switch(mov) {
        case Movimento::U: return Movimento::u;
        case Movimento::u: return Movimento::U;
        case Movimento::L: return Movimento::l;
        case Movimento::l: return Movimento::L;
        case Movimento::F: return Movimento::f;
        case Movimento::f: return Movimento::F;
        default: return mov;
    }
}

inline char movimento_to_char(Movimento mov) {
    switch(mov) {
        case Movimento::U: return 'U';
        case Movimento::u: return 'u';
        case Movimento::L: return 'L';
        case Movimento::l: return 'l';
        case Movimento::F: return 'F';
        case Movimento::f: return 'f';
        default: return '?';
    }
}

// --- Estrutura do estado decodificado (para manipulação) ---

// --- Estado decodificado (para pré-computação) ---
struct EstadoDecodificado {
    std::array<uint8_t,8> pos; // permutação das 7 peças (DRB é implícita)
    std::array<uint8_t,8> ori; // orientações (DRB é implícita)

    static EstadoDecodificado aplicarMovimento(const EstadoDecodificado& s, Movimento mov) {
        EstadoDecodificado r = s;

        switch(mov) {
            case Movimento::U: // U
                r.pos[0] = s.pos[3];
                r.pos[1] = s.pos[0];
                r.pos[2] = s.pos[1];
                r.pos[3] = s.pos[2];

                r.ori[0] = s.ori[3];
                r.ori[1] = s.ori[0];
                r.ori[2] = s.ori[1];
                r.ori[3] = s.ori[2];
                break;

            case Movimento::u: // U'
                r.pos[0] = s.pos[1];
                r.pos[1] = s.pos[2];
                r.pos[2] = s.pos[3];
                r.pos[3] = s.pos[0];

                r.ori[0] = s.ori[1];
                r.ori[1] = s.ori[2];
                r.ori[2] = s.ori[3];
                r.ori[3] = s.ori[0];
                break;

            case Movimento::L: // L
                r.pos[1] = s.pos[2];
                r.pos[2] = s.pos[6];
                r.pos[6] = s.pos[5];
                r.pos[5] = s.pos[1];

                r.ori[1] = (s.ori[2] + 1) % 3;
                r.ori[2] = (s.ori[6] + 2) % 3;
                r.ori[6] = (s.ori[5] + 1) % 3;
                r.ori[5] = (s.ori[1] + 2) % 3;
                break;

            case Movimento::l: // L'
                r.pos[1] = s.pos[5];
                r.pos[2] = s.pos[1];
                r.pos[6] = s.pos[2];
                r.pos[5] = s.pos[6];

                r.ori[1] = (s.ori[5] + 1) % 3;
                r.ori[2] = (s.ori[1] + 2) % 3;
                r.ori[6] = (s.ori[2] + 1) % 3;
                r.ori[5] = (s.ori[6] + 2) % 3;
                break;

            case Movimento::F: // F
                r.pos[0] = s.pos[1];
                r.pos[1] = s.pos[5];
                r.pos[5] = s.pos[4];
                r.pos[4] = s.pos[0];

                r.ori[0] = (s.ori[1] + 1) % 3;
                r.ori[1] = (s.ori[5] + 2) % 3;
                r.ori[5] = (s.ori[4] + 1) % 3;
                r.ori[4] = (s.ori[0] + 2) % 3;
                break;

            case Movimento::f: // F'
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
                break;
        }
        return r;
    }
};

// --- Estrutura do estado do cubo ---

constexpr int N_ORI  = 729;  // 3^7
constexpr int N_PERM = 5040;  // 7!
constexpr int N_MOV  = 6;     // U, u, L, l, F, f

// Tabelas carregadas na memória
extern uint16_t oriMove[N_ORI][N_MOV];
extern uint16_t permMove[N_PERM][N_MOV];

// Funções do cubo
void carregarTabelas();
uint32_t packState(uint16_t ori, uint16_t perm);

struct EstadoCodificado {
    uint16_t oriCoord;   // 0..729 (3^6 - 1)
    uint16_t permCoord;  // 0..5039 (7! - 1)

    bool operator==(const EstadoCodificado& rhs) const {
        return permCoord == rhs.permCoord && oriCoord == rhs.oriCoord;
    }

    // Aplica um movimento ao estado e retorna o novo estado
    static EstadoCodificado aplicarMovimento(const EstadoCodificado& s, Movimento mov) {
        EstadoCodificado r;
        r.oriCoord = oriMove[s.oriCoord][static_cast<uint8_t>(mov)];
        r.permCoord = permMove[s.permCoord][static_cast<uint8_t>(mov)];
        return r;
    }

    // --- Conversões orientação ---
    static uint16_t oriToCoord(const std::array<uint8_t,8>& ori) {
        int coord = 0;
        for (int i = 0; i < 6; i++) {
            coord = coord * 3 + ori[i];
        }
        return coord;
    }

    static std::array<uint8_t,8> coordToOri(uint16_t coord) {
        std::array<uint8_t,8> ori{};
        int sum = 0;
        for (int i = 5; i >= 0; i--) {
            ori[i] = coord % 3;
            sum += ori[i];
            coord /= 3;
        }
        ori[6] = (3 - (sum % 3)) % 3; // DLB deduzido
        ori[7] = 0; // DRB implícito
        return ori;
    }

    // --- Conversões permutação (Lehmer code) ---
    static uint16_t permToCoord(const std::array<uint8_t,8>& pos) {
        uint16_t coord = 0;
        int fact = 1;
        for (int i = 0; i < 7; i++) {
            int smaller = 0;
            for (int j = i+1; j < 7; j++) {
                if (pos[j] < pos[i]) smaller++;
            }
            coord = coord * (7 - i) + smaller;
        }
        return coord;
    }


    static std::array<uint8_t,8> coordToPerm(uint16_t coord) {
        std::array<uint8_t,8> pos{};
        std::vector<int> elems = {0,1,2,3,4,5,6,7};

        for (int i = 0; i < 7; i++) {
            int fact = 1;
            for (int k = 2; k <= 6 - i; k++) fact *= k;  // (7-i)! 
            int idx = coord / fact;
            coord %= fact;
            pos[i] = elems[idx];
            elems.erase(elems.begin() + idx);
        }
        pos[7] = elems[0]; // DRB implícito
        return pos;
    }

};
