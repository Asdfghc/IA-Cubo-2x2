#include <array>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>

// --- Constantes de movimentos ---
constexpr char movimentos[] = {'U', 'u', 'L', 'l', 'F', 'f'};
constexpr int N_MOV = 6;

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

// --- Estado decodificado (para pré-computação) ---
struct EstadoDecodificado {
    std::array<uint8_t,8> pos; // permutação das 7 peças (DRB é implícita)
    std::array<uint8_t,8> ori; // orientações (DRB é implícita)

    static EstadoDecodificado aplicarMovimento(const EstadoDecodificado& s, char mov) {
        EstadoDecodificado r = s;

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
        }
        return r;
    }
};

// --- Conversões orientação ---
uint16_t oriToCoord(const std::array<uint8_t,8>& ori) {
    int coord = 0;
    for (int i = 6; i >= 0; i--) {
        coord = coord * 3 + ori[i];
    }
    return coord;
}

std::array<uint8_t,8> coordToOri(uint16_t coord) {
    std::array<uint8_t,8> ori{};
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        ori[i] = coord % 3;
        sum += ori[i];
        coord /= 3;
    }
    ori[7] = (3 - (sum % 3)) % 3; // DRB implícito
    return ori;
}

// --- Conversões permutação (Lehmer code) ---
uint16_t permToCoord(const std::array<uint8_t,8>& pos) {
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


std::array<uint8_t,8> coordToPerm(uint16_t coord) {
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


// --- Tabelas ---
constexpr int N_ORI  = 729;  // 3^7
constexpr int N_PERM = 5040;  // 7!
uint16_t oriMove[N_ORI][N_MOV];
uint16_t permMove[N_PERM][N_MOV];

// --- Pré-computação das move tables ---
void gerarMoveTables() {
    // Orientações
    for (int i = 0; i < N_ORI; i++) {
        EstadoDecodificado s;
        s.pos = {0,1,2,3,4,5,6,7};
        s.ori = coordToOri(i);

        for (int m = 0; m < N_MOV; m++) {
            auto r = EstadoDecodificado::aplicarMovimento(s, movimentos[m]);
            oriMove[i][m] = oriToCoord(r.ori);
        }
    }

    // Permutações
    for (int i = 0; i < N_PERM; i++) {
        EstadoDecodificado s;
        s.pos = coordToPerm(i);
        s.ori = {0,0,0,0,0,0,0,0};

        for (int m = 0; m < N_MOV; m++) {
            auto r = EstadoDecodificado::aplicarMovimento(s, movimentos[m]);
            permMove[i][m] = permToCoord(r.pos);
        }
    }
}

// --- Salvar tabelas ---
void salvarTabelas() {
    std::ofstream f1("tables/oriMove.bin", std::ios::binary);
    f1.write((char*)oriMove, sizeof(oriMove));
    f1.close();

    std::ofstream f2("tables/permMove.bin", std::ios::binary);
    f2.write((char*)permMove, sizeof(permMove));
    f2.close();
}

int main() {
    gerarMoveTables();
    salvarTabelas();
    std::cout << "Tabelas geradas e salvas!" << std::endl;
}
