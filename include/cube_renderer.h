#pragma once
#include <array>
#include "cube_state.h"

using namespace std;

// Enumerações e tipos auxiliares

enum class Color {
    None,
    White, Yellow,
    Green, Blue,
    Orange, Red
};

struct RGB {
    float r, g, b;
};

enum class Face { F=0, B=1, L=2, R=3, U=4, D=5 };

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

// Define as cores globais associadas a cada canto (posição 0..7) e orientação (0..2)
inline const Color cornerColorsGlobal[8][3][3] = {
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
inline const array<array<Face,3>,8> POS_BASE_FACES = {{
    {Face::U,Face::R,Face::F}, // UFR
    {Face::U,Face::F,Face::L}, // UFL
    {Face::U,Face::L,Face::B}, // UBL
    {Face::U,Face::B,Face::R}, // UBR
    {Face::D,Face::F,Face::R}, // DFR
    {Face::D,Face::L,Face::F}, // DFL
    {Face::D,Face::B,Face::L}, // DBL
    {Face::D,Face::R,Face::B}  // DBR
}};

// Definição das cores básicas de cada canto no estado resolvido
// ordem: {marcador U/D, lateral1, lateral2}
inline const array<array<Color, 3>, 8> cornerBase = {{
    { Color::White, Color::Red, Color::Green},   // 0: UFR
    { Color::White, Color::Green, Color::Orange}, // 1: UFL
    { Color::White, Color::Orange, Color::Blue},  // 2: UBL
    { Color::White, Color::Blue, Color::Red},     // 3: UBR
    { Color::Yellow, Color::Green, Color::Red},   // 4: DFR
    { Color::Yellow, Color::Orange, Color::Green}, // 5: DFL
    { Color::Yellow, Color::Blue, Color::Orange},  // 6: DBL
    { Color::Yellow, Color::Red, Color::Blue}      // 7: DBR
}};

// Funções principais
void drawCubie(float x, float y, float z, const array<Color, 6>& stickers);
int findPieceAtPos(const EstadoDecodificado& e, int pos);
array<Color,3> getCornerColors(int cubieId, int ori);
array<array<Color,6>,8> getStickersForState(const EstadoDecodificado& estado);