#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "cube_state.h"
#include <iostream>
#include "solver.h"

using namespace std;

// Variáveis globais para controle de rotação e estado do cubo
float angle_horizontal = 0.0f;
float angle_vertical = 0.0f;
array<array<Color, 6>, 8> stickers;
Estado estado;

// --- Funções utilitárias de desenho ---

// Desenha um cubinho individual na posição (x, y, z) com as cores dos stickers
void drawCubie(float x, float y, float z, const array<Color, 6>& stickers) {
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

// --- Funções de callback do GLUT ---

// Função de desenho principal
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Configura a câmera
    glTranslatef(0, 0, -6);
    glRotatef(angle_horizontal, 0, 1, 0);
    float rad_v = angle_horizontal * M_PI / 180.0f;
    glRotatef(angle_vertical, -cos(rad_v), 0, -sin(rad_v));

    // Desenha os 8 cubinhos (2x2x2)
    const float offset = 0.52f; // distância entre centros dos cubinhos
    const float positions[8][3] = {
        { offset,  offset,  offset},
        {-offset,  offset,  offset},
        {-offset,  offset, -offset},
        { offset,  offset, -offset},
        { offset, -offset,  offset},
        {-offset, -offset,  offset},
        {-offset, -offset, -offset},
        { offset, -offset, -offset}
    };
    for (int i = 0; i < 8; i++) {
        drawCubie(positions[i][0], positions[i][1], positions[i][2], stickers[i]);
    }

    glutSwapBuffers();
}

// Função chamada ao redimensionar a janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Callback para teclas especiais (setas)
void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            angle_horizontal -= 10.0f;
            if (angle_horizontal < 0) angle_horizontal += 360;
            break;
        case GLUT_KEY_RIGHT:
            angle_horizontal += 10.0f;
            if (angle_horizontal > 360) angle_horizontal -= 360;
            break;
        case GLUT_KEY_DOWN:
            angle_vertical -= 10.0f;
            if (angle_vertical < 0) angle_vertical += 360;
            break;
        case GLUT_KEY_UP:
            angle_vertical += 10.0f;
            if (angle_vertical > 360) angle_vertical -= 360;
            break;
    }
    glutPostRedisplay();
}

// Callback para teclas normais (letras)
void keyboardChar(unsigned char key, int x, int y) {
    if (key == 13) {
        Estado estado_inicial = estado;
        vector<char> caminho;
        
        bool achou = solve_bfs(estado_inicial, caminho);
        if (achou) {
            cout << "achou: ";
            for (char mov : caminho) {
                cout << mov << " ";
            }
        } else {
            cout << "nao achou";
        }
        cout << endl;
    }
    estado = Estado::aplicarMovimento(estado, key);
    stickers = getStickersForState(estado);
    glutPostRedisplay();
}

// --- Função principal ---

int main(int argc, char** argv) {
    // Estado inicial do cubo
    estado = {{0,1,2,3,4,5,6}, {0,0,0,0,0,0,0}};
    stickers = getStickersForState(estado);

    // Inicialização do GLUT e OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cubo Magico 2x2");

    glEnable(GL_DEPTH_TEST);

    // Registra callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutKeyboardFunc(keyboardChar);

    glutMainLoop();
    return 0;
}
