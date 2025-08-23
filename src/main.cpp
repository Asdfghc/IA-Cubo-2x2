#include <GL/glut.h>

float angle = 0.0f;

void drawCubie(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);

    // Frente (vermelho)
    glColor3f(1,0,0);
    glVertex3f(-0.5,-0.5, 0.5);
    glVertex3f( 0.5,-0.5, 0.5);
    glVertex3f( 0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Trás (laranja)
    glColor3f(1,0.5,0);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(-0.5, 0.5,-0.5);
    glVertex3f( 0.5, 0.5,-0.5);
    glVertex3f( 0.5,-0.5,-0.5);

    // Esquerda (verde)
    glColor3f(0,1,0);
    glVertex3f(-0.5,-0.5,-0.5);
    glVertex3f(-0.5,-0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5,-0.5);

    // Direita (azul)
    glColor3f(0,0,1);
    glVertex3f(0.5,-0.5,-0.5);
    glVertex3f(0.5, 0.5,-0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5,-0.5, 0.5);

    // Topo (branco)
    glColor3f(1,1,1);
    glVertex3f(-0.5, 0.5,-0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f( 0.5, 0.5, 0.5);
    glVertex3f( 0.5, 0.5,-0.5);

    // Fundo (amarelo)
    glColor3f(1,1,0);
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
    glRotatef(angle,1,1,0);

    // desenha os 8 cubinhos (2x2x2) mais próximos
    float offset = 0.52f; // distância entre centros dos cubinhos
    for (float x = -offset; x <= offset; x += 2*offset)
        for (float y = -offset; y <= offset; y += 2*offset)
            for (float z = -offset; z <= offset; z += 2*offset)
                drawCubie(x, y, z);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    angle += 1.0f;  // gira
    if(angle > 360) angle -= 360;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("Cubo Magico 2x2");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
