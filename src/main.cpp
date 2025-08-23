#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>

struct Vec3 {
    float x, y, z;
};

struct Color {
    int r, g, b;
};

struct Vertex {
    Vec3 pos;
    Color color;
};

struct Quad {
    Vertex vertices[4];
};

// Terminal buffer
const int WIDTH = 80;
const int HEIGHT = 40;
char screen[HEIGHT][WIDTH];
float zbuffer[HEIGHT][WIDTH];

// Projeção perspectiva simples
Vec3 project(const Vec3& v, float fov, float aspect, float zNear) {
    float scale = fov / (v.z + zNear);
    return {v.x * scale * aspect, v.y * scale, v.z};
}

// Transformações
Vec3 rotateY(const Vec3& v, float angle) {
    float c = cos(angle), s = sin(angle);
    return {v.x * c - v.z * s, v.y, v.x * s + v.z * c};
}

// Desenha um ponto no buffer
void drawPoint(const Vec3& v, const Color& col) {
    int x = (int)((v.x + 1) * 0.5f * WIDTH);
    int y = (int)((1 - (v.y + 1) * 0.5f) * HEIGHT);
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (v.z > zbuffer[y][x]) {
            zbuffer[y][x] = v.z;
            std::cout << "\033[" << y+1 << ";" << x+1 << "H"; // Move cursor
            std::cout << "\033[38;2;" << col.r << ";" << col.g << ";" << col.b << "m" << "#" << "\033[0m";
        }
    }
}

// Renderiza um quad (apenas vértices, depois pode interpolar)
void drawQuad(const Quad& q, float fov, float aspect, float zNear) {
    for (int i = 0; i < 4; ++i) {
        Vec3 p = project(q.vertices[i].pos, fov, aspect, zNear);
        drawPoint(p, q.vertices[i].color);
    }
}

// Limpa o terminal e buffers
void clearScreen() {
    std::cout << "\033[2J"; // Clear screen
    std::cout << "\033[H";  // Move cursor home
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            zbuffer[y][x] = -1e9;
}

int main() {
    // Define cubo
    std::vector<Quad> cube;
    Color colors[6] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{0,255,255},{255,0,255}};

    Vec3 verts[8] = {
        {-0.5f,-0.5f,-0.5f},{0.5f,-0.5f,-0.5f},{0.5f,0.5f,-0.5f},{-0.5f,0.5f,-0.5f},
        {-0.5f,-0.5f,0.5f},{0.5f,-0.5f,0.5f},{0.5f,0.5f,0.5f},{-0.5f,0.5f,0.5f}
    };

    int faces[6][4] = {
        {0,1,2,3},{4,5,6,7},{0,1,5,4},{2,3,7,6},{0,3,7,4},{1,2,6,5}
    };

    for (int f = 0; f < 6; ++f) {
        Quad q;
        for (int i = 0; i < 4; ++i) {
            q.vertices[i].pos = verts[faces[f][i]];
            q.vertices[i].color = colors[f];
        }
        cube.push_back(q);
    }

    float angle = 0.0f;
    while (true) {
        clearScreen();

        // Rotaciona cubo
        std::vector<Quad> transformed = cube;
        for (auto& q : transformed)
            for (auto& v : q.vertices)
                v.pos = rotateY(v.pos, angle);

        for (auto& q : transformed)
            drawQuad(q, 1.0f, float(WIDTH)/HEIGHT, 3.0f);

        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        angle += 0.05f;
    }
}
