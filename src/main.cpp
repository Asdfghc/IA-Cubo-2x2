#include <GL/glut.h>
#include <cmath>
#include "cube_state.h"

float angle_horizontal = 0.0f;
float angle_vertical = 0.0f;
std::array<std::array<Color, 6>, 8> colors;
Estado estado;

void drawCubie(float x, float y, float z, std::array<Color, 6> colors) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);

    // Frente
    RGB frontColor = colorToRGB(colors[0]);
    glColor3f(frontColor.r, frontColor.g, frontColor.b);
    glVertex3f(-0.5,-0.5, 0.5);
    glVertex3f( 0.5,-0.5, 0.5);
    glVertex3f( 0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Trás (laranja)
    RGB backColor = colorToRGB(colors[1]);
    glColor3f(backColor.r, backColor.g, backColor.b);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(-0.5, 0.5,-0.5);
    glVertex3f( 0.5, 0.5,-0.5);
    glVertex3f( 0.5,-0.5,-0.5);

    // Esquerda (verde)
    RGB leftColor = colorToRGB(colors[2]);
    glColor3f(leftColor.r, leftColor.g, leftColor.b);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(-0.5,-0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5,-0.5);

    // Direita (azul)
    RGB rightColor = colorToRGB(colors[3]);
    glColor3f(rightColor.r, rightColor.g, rightColor.b);
    glVertex3f(0.5,-0.5,-0.5);
    glVertex3f(0.5, 0.5,-0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5,-0.5, 0.5);

    // Topo (branco)
    RGB topColor = colorToRGB(colors[4]);
    glColor3f(topColor.r, topColor.g, topColor.b);
    glVertex3f(-0.5, 0.5,-0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f( 0.5, 0.5, 0.5);
    glVertex3f( 0.5, 0.5,-0.5);

    // Fundo (amarelo)
    RGB bottomColor = colorToRGB(colors[5]);
    glColor3f(bottomColor.r, bottomColor.g, bottomColor.b);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f( 0.5,-0.5,-0.5);
    glVertex3f( 0.5,-0.5, 0.5);
    glVertex3f(-0.5,-0.5, 0.5);

    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // câmera
    glTranslatef(0,0,-6);
    glRotatef(angle_horizontal,0,1,0);
    float rad_v = angle_horizontal * M_PI / 180.0f;
    glRotatef(angle_vertical, -cos(rad_v), 0, -sin(rad_v));

    // desenha os 8 cubinhos (2x2x2) mais próximos
    float offset = 0.52f; // distância entre centros dos cubinhos
    int piece = 0;
    float positions[8][3] = {
        { offset,  offset,  offset},
        {-offset,  offset,  offset},
        {-offset,  offset, -offset},
        { offset,  offset, -offset},
        { offset,  -offset,  offset},
        {-offset,  -offset,  offset},
        {-offset,  -offset, -offset},
        { offset,  -offset, -offset}
    };
    for (int i = 0; i < 8; i++) {
        drawCubie(positions[i][0], positions[i][1], positions[i][2], colors[piece++]);
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}


void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            angle_horizontal -= 10.0f;
            if (angle_horizontal < 0) angle_horizontal += 360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            angle_horizontal += 10.0f;
            if (angle_horizontal > 360) angle_horizontal -= 360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            angle_vertical -= 10.0f;
            if (angle_vertical < 0) angle_vertical += 360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            angle_vertical += 10.0f;
            if (angle_vertical > 360) angle_vertical -= 360;
            glutPostRedisplay();
            break;
    }
}

void keyboardChar(unsigned char key, int x, int y) {
    if (key == 'U' || key == 'u' || key == 'L' || key == 'l' || key == 'F' || key == 'f' || key == 'R' || key == 'r') {
        estado = Estado::aplicarMovimento(estado, key);
        colors = getColorsForState(estado);
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    estado = {{0,1,2,3,4,5,6}, {0,0,0,0,0,0,0}};
    colors = getColorsForState(estado);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("Cubo Magico 2x2");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard); // teclas especiais (setas)
    glutKeyboardFunc(keyboardChar); // teclas normais (letras)
    glutMainLoop();
    return 0;
}
