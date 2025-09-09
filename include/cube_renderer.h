#pragma once
#include <array>
#include "cube_state.h"

// --- Enumerações e tipos auxiliares ---

enum class Color {
    None,
    White, Yellow,
    Green, Blue,
    Orange, Red
};

struct RGB {
    float r, g, b;
};

enum Face { F=0, B=1, L=2, R=3, U=4, D=5 };

// --- Funções utilitárias de cor ---

inline RGB colorToRGB(Color c) {
    switch (c) {
        case Color::White:  return {1.0f, 1.0f, 1.0f};
        case Color::Yellow: return {1.0f, 1.0f, 0.0f};
        case Color::Red:    return {1.0f, 0.0f, 0.0f};
        case Color::Orange: return {1.0f, 0.5f, 0.0f};
        case Color::Blue:   return {0.0f, 0.0f, 1.0f};
        case Color::Green:  return {0.0f, 1.0f, 0.0f};
        case Color::None:   return {0.1f, 0.1f, 0.1f};
    }
    return {0.0f, 0.0f, 0.0f};
}


// --- Constantes de cubos e faces ---

// cores por cubie (0..7) e orientação (0..2), já mapeadas para as faces globais
static const Color cornerColorsGlobal[8][3][3] = {
    // UFR
    { {Color::White, Color::Red, Color::Green},   // ori=0
      {Color::Green, Color::White, Color::Red},   // ori=1
      {Color::Red, Color::Green, Color::White} }, // ori=2
    // UFL
    { {Color::White, Color::Green, Color::Orange},
      {Color::Orange, Color::White, Color::Green},
      {Color::Green, Color::Orange, Color::White} },
    // UBL
    { {Color::White, Color::Orange, Color::Blue},
      {Color::Blue, Color::White, Color::Orange},
      {Color::Orange, Color::Blue, Color::White} },
    // UBR
    { {Color::White, Color::Blue, Color::Red},
      {Color::Red, Color::White, Color::Blue},
      {Color::Blue, Color::Red, Color::White} },
    // DFR
    { {Color::Yellow, Color::Green, Color::Red},
      {Color::Red, Color::Yellow, Color::Green},
      {Color::Green, Color::Red, Color::Yellow} },
    // DFL
    { {Color::Yellow, Color::Orange, Color::Green},
      {Color::Green, Color::Yellow, Color::Orange},
      {Color::Orange, Color::Green, Color::Yellow} },
    // DBL
    { {Color::Yellow, Color::Blue, Color::Orange},
      {Color::Orange, Color::Yellow, Color::Blue},
      {Color::Blue, Color::Orange, Color::Yellow} },
    // DRB
    { {Color::Yellow, Color::Red, Color::Blue},
      {Color::Blue, Color::Yellow, Color::Red},
      {Color::Red, Color::Blue, Color::Yellow} }
};

// Define as faces globais associadas a cada posição (0..7)
static const std::array<std::array<Face,3>,8> POS_BASE_FACES = {{
    {U,R,F}, // UFR
    {U,F,L}, // UFL
    {U,L,B}, // UBL
    {U,B,R}, // UBR
    {D,F,R}, // DFR
    {D,L,F}, // DFL
    {D,B,L}, // DBL
    {D,R,B}  // DBR
}};

// Definição das cores básicas de cada canto no estado resolvido
// ordem: {marcador U/D, lateral1, lateral2}
static const std::array<std::array<Color, 3>, 8> cornerBase = {{
    { Color::White, Color::Red, Color::Green},   // 0: UFR
    { Color::White, Color::Green, Color::Orange}, // 1: UFL
    { Color::White, Color::Orange, Color::Blue},  // 2: UBL
    { Color::White, Color::Blue, Color::Red},     // 3: UBR
    { Color::Yellow, Color::Green, Color::Red},   // 4: DFR
    { Color::Yellow, Color::Orange, Color::Green}, // 5: DFL
    { Color::Yellow, Color::Blue, Color::Orange},  // 6: DBL
    { Color::Yellow, Color::Red, Color::Blue}      // 7: DBR
}};

// --- Funções utilitárias de cubo ---

// Busca qual PEÇA está em uma posição global (0..6). Retorna -1 se não achou.
inline int findPieceAtPos(const Estado& e, int pos) {
    for (int piece = 0; piece < 7; ++piece)
        if (e.pos[piece] == pos) return piece;
    return -1;
}

// Retorna as cores do cubie rotacionadas conforme a orientação
inline std::array<Color,3> getCornerColors(int cubieId, int ori) {
    std::array<Color,3> base = cornerBase[cubieId];
    std::array<Color,3> rotated;
    for(int i=0;i<3;i++){
        rotated[i] = base[(i + ori) % 3];
    }
    return rotated;
}

// Retorna as cores dos stickers para cada face global de cada cubie
inline std::array<std::array<Color,6>,8> getStickersForState(const Estado& estado) {
    std::array<std::array<Color,6>,8> out;
    for (int pos=0; pos<8; pos++) {
        out[pos].fill(Color::None);
    }
    for (int pos=0; pos<7; pos++) {
        int cubieId = estado.pos[pos];
        int ori     = estado.ori[pos];
        for (int k=0; k<3; k++) {
            Face f = POS_BASE_FACES[pos][k];
            out[pos][f] = cornerColorsGlobal[cubieId][ori][k];
        }
    }
    // DBR (posição 7) sempre tem as cores base
    for (int l=0; l<3; l++) {
        Face f = POS_BASE_FACES[7][l];
        out[7][f] = cornerBase[7][l];
    }
    return out;
}