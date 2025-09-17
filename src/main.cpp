#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "cube_state.h"
#include "cube_renderer.h"
#include <iostream>
#include <string>
#include "solver.h"

using namespace std;

// Variáveis globais para controle de rotação e estado do cubo
float angle_horizontal = 0.0f;
float angle_vertical = 0.0f;
array<array<Color, 6>, 8> stickers;
EstadoDecodificado estado;
string overlay_message = "";

// Funções de callback do GLUT

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

    // Desenha mensagem de overlay (texto 2D)
    if (!overlay_message.empty()) {
        // Salva as matrizes atuais
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        gluOrtho2D(0, viewport[2], 0, viewport[3]);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Define cor do texto (vermelho)
        glColor3f(1.0f, 0.0f, 0.0f);
        // Posição: canto superior esquerdo
        float x = 10.0f;
        float y = viewport[3] - 30.0f;
        glRasterPos2f(x, y);
        for (const char* c = overlay_message.c_str(); *c != '\0'; ++c) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }

        // Restaura as matrizes
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
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
        // Converte estado decodificado para codificado
        EstadoCodificado codificado;
        codificado.oriCoord = EstadoCodificado::oriToCoord(estado.ori);
        codificado.permCoord = EstadoCodificado::permToCoord(estado.pos);

        // Chama o solver no estado codificado
        overlay_message = solver_wrapper(codificado, "astar", 0);

    } else if (key == 'U') {
        estado = EstadoDecodificado::aplicarMovimento(estado, Movimento::U);
    } else if (key == 'u') {
        estado = EstadoDecodificado::aplicarMovimento(estado, Movimento::u);
    } else if (key == 'L') {
        estado = EstadoDecodificado::aplicarMovimento(estado, Movimento::L);
    } else if (key == 'l') {
        estado = EstadoDecodificado::aplicarMovimento(estado, Movimento::l);
    } else if (key == 'F') {
        estado = EstadoDecodificado::aplicarMovimento(estado, Movimento::F);
    } else if (key == 'f') {
        estado = EstadoDecodificado::aplicarMovimento(estado, Movimento::f);
    }
    stickers = getStickersForState(estado);
    glutPostRedisplay();
}


int main(int argc, char** argv) {
    carregarTabelas();
    // Estado inicial do cubo
    estado = {{0,1,2,3,4,5,6,7}, {0,0,0,0,0,0,0,0}};
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

    // Inicia o loop principal
    glutMainLoop();
    return 0;
}
