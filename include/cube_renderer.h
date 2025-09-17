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

enum class Face { F=0, B=1, L=2, R=3, U=4, D=5 };


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


void drawCubie(float x, float y, float z, const std::array<Color, 6>& stickers);
RGB colorToRGB(Color c);
int findPieceAtPos(const EstadoDecodificado& e, int pos);
std::array<Color,3> getCornerColors(int cubieId, int ori);
std::array<std::array<Color,6>,8> getStickersForState(const EstadoDecodificado& estado);