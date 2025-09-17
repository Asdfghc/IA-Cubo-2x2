#include "cube_renderer.h"
#include <GL/glut.h>

// Desenha um cubinho individual na posição (x, y, z) com as cores dos stickers
void drawCubie(float x, float y, float z, const std::array<Color, 6>& stickers) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);

    // Frente
    RGB frontColor = colorToRGB(stickers[0]);
    glColor3f(frontColor.r, frontColor.g, frontColor.b);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f( 0.5, -0.5, 0.5);
    glVertex3f( 0.5,  0.5, 0.5);
    glVertex3f(-0.5,  0.5, 0.5);

    // Trás
    RGB backColor = colorToRGB(stickers[1]);
    glColor3f(backColor.r, backColor.g, backColor.b);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f( 0.5,  0.5, -0.5);
    glVertex3f( 0.5, -0.5, -0.5);

    // Esquerda
    RGB leftColor = colorToRGB(stickers[2]);
    glColor3f(leftColor.r, leftColor.g, leftColor.b);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f(-0.5,  0.5,  0.5);
    glVertex3f(-0.5,  0.5, -0.5);

    // Direita
    RGB rightColor = colorToRGB(stickers[3]);
    glColor3f(rightColor.r, rightColor.g, rightColor.b);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5,  0.5, -0.5);
    glVertex3f(0.5,  0.5,  0.5);
    glVertex3f(0.5, -0.5,  0.5);

    // Topo
    RGB topColor = colorToRGB(stickers[4]);
    glColor3f(topColor.r, topColor.g, topColor.b);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5,  0.5);
    glVertex3f( 0.5, 0.5,  0.5);
    glVertex3f( 0.5, 0.5, -0.5);

    // Fundo
    RGB bottomColor = colorToRGB(stickers[5]);
    glColor3f(bottomColor.r, bottomColor.g, bottomColor.b);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f( 0.5, -0.5, -0.5);
    glVertex3f( 0.5, -0.5,  0.5);
    glVertex3f(-0.5, -0.5,  0.5);

    glEnd();
    glPopMatrix();
}


RGB colorToRGB(Color c) {
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

// --- Funções utilitárias de cubo ---

// Busca qual PEÇA está em uma posição global (0..6). Retorna -1 se não achou.
int findPieceAtPos(const EstadoDecodificado& e, int pos) {
    for (int piece = 0; piece < 7; ++piece)
        if (e.pos[piece] == pos) return piece;
    return -1;
}

// Retorna as cores do cubie rotacionadas conforme a orientação
std::array<Color,3> getCornerColors(int cubieId, int ori) {
    std::array<Color,3> base = cornerBase[cubieId];
    std::array<Color,3> rotated;
    for(int i=0;i<3;i++){
        rotated[i] = base[(i + ori) % 3];
    }
    return rotated;
}

// Retorna as cores dos stickers para cada face global de cada cubie
std::array<std::array<Color,6>,8> getStickersForState(const EstadoDecodificado& estado) {
    std::array<std::array<Color,6>,8> out;
    for (int pos=0; pos<8; pos++) {
        out[pos].fill(Color::None);
    }
    for (int pos=0; pos<7; pos++) {
        int cubieId = estado.pos[pos];
        int ori     = estado.ori[pos];
        for (int k=0; k<3; k++) {
            Face f = POS_BASE_FACES[pos][k];
            out[pos][static_cast<int>(f)] = cornerColorsGlobal[cubieId][ori][k];
        }
    }
    // DBR (posição 7) sempre tem as cores base
    for (int l=0; l<3; l++) {
        Face f = POS_BASE_FACES[7][l];
        out[7][static_cast<int>(f)] = cornerBase[7][l];
    }
    return out;
}