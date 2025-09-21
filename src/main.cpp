#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "cube_state.h"
#include "cube_renderer.h"
#include <iostream>
#include <string>
#include "solver.h"

using namespace std;

struct Button {
    float x, y, w, h;   // Coordenadas
    string label;  
    string solver; // "astar", "bfs", "dfs"
};

// Variáveis globais para controle de rotação e estado do cubo
float angle_horizontal = 28.0f;
float angle_vertical = -21.0f;
array<array<Color, 6>, 8> stickers;
EstadoDecodificado estado;
string overlay_message = "";

//Variáveis globais para controle da janela
int window_width = 600;
int window_height = 600;

// Botões para seleção de algoritmo
vector<Button> buttons = {
    {500, 20, 100, 30, "A*", "astar"},
    {500, 140, 100, 30, "BFS", "bfs"},
    {500, 260, 100, 30, "DFS", "dfs"}
};

void drawButton(const Button& btn);

// Funções de callback do GLUT

// Função de desenho principal
void display() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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

    // Overlay 2D para botões e mensagem
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(0, viewport[2], 0, viewport[3]);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    // Desenha botões
    for (const auto& btn : buttons)
        drawButton(btn);

    //Mensagem do Menu
    string menu_msg = "Use as setas para girar o cubo.\n"
                      "Movimentos: L, l, U, u, F, f\n"
                      "Clique nos botoes para escolher o algoritmo.";

    glColor3f(0.9f, 0.9f, 0.9f);

    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);

    float x = w * 0.030f;
    float y = h * 0.10f;

    // Desenha cada linha do menu
    size_t start = 0;
    size_t end;
    int line_height = h * 0.035f;

    while ((end = menu_msg.find('\n', start)) != string::npos) {
        string line = menu_msg.substr(start, end - start);
        glRasterPos2f(x, y);
        for (const char* c = line.c_str(); *c != '\0'; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        y -= line_height;
        start = end + 1;
    }
    
    // Última linha
    glRasterPos2f(x, y);
    for (const char* c = menu_msg.c_str() + start; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Mensagem de overlay (vermelha)
    if (!overlay_message.empty()) {
        glColor3f(1.0f, 0.0f, 0.0f);
        float x = 10.0f;
        float y = viewport[3] - 30.0f;
        glRasterPos2f(x, y);
        for (const char* c = overlay_message.c_str(); *c != '\0'; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}

void drawButton(const Button& btn) {
    // Retângulo do botão
    glColor3f(0.2f, 0.2f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(btn.x, btn.y);
    glVertex2f(btn.x + btn.w, btn.y);
    glVertex2f(btn.x + btn.w, btn.y + btn.h);
    glVertex2f(btn.x, btn.y + btn.h);
    glEnd();

    // Borda
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(btn.x, btn.y);
    glVertex2f(btn.x + btn.w, btn.y);
    glVertex2f(btn.x + btn.w, btn.y + btn.h);
    glVertex2f(btn.x, btn.y + btn.h);
    glEnd();

    // Texto
    glColor3f(1,1,1);

    int text_width = 0;

    for (const char* c = btn.label.c_str(); *c; ++c)
        text_width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c);
    
    float text_x = btn.x + (btn.w - text_width) / 2;
    float text_y = btn.y + (btn.h - 16) / 2;
    glRasterPos2f(text_x, text_y);
    for (const char* c = btn.label.c_str(); *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

// Função chamada ao redimensionar a janela
void reshape(int w, int h) {
    window_width = w;
    window_height = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // Atualiza posições dos botões
    int margin = 20;     
    int btn_width = 100;  
    int btn_height = 30;  
    int spacing = 10;

    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].x = w - btn_width - margin;
        buttons[i].y = margin + i * (btn_height + spacing);
        buttons[i].w = btn_width;
        buttons[i].h = btn_height;
    }
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

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        int win_h = viewport[3];
        y = win_h - y;

        for (const auto& btn : buttons) {
            if (x >= btn.x && x <= btn.x + btn.w && y >= btn.y && y <= btn.y + btn.h) {
                EstadoCodificado codificado;
                codificado.oriCoord = EstadoCodificado::oriToCoord(estado.ori);
                codificado.permCoord = EstadoCodificado::permToCoord(estado.pos);
                overlay_message = solver_wrapper(codificado, btn.solver, 0);
                glutPostRedisplay();
                break;
            }
        }
    }
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
    glutMouseFunc(mouse);

    // Inicia o loop principal
    glutMainLoop();
    return 0;
}