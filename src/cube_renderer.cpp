#include "cube_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

// Um cubo com cores por face (cada face duplicada)
// Um cubo com faces independentes
// Layout: pos (3) | normal (3) | faceId (1)
static float cubeVertices[] = {
    // Face -Z (traseira) id=0
   -0.5f,-0.5f,-0.5f,   0,0,-1,   0,
    0.5f,-0.5f,-0.5f,   0,0,-1,   0,
    0.5f, 0.5f,-0.5f,   0,0,-1,   0,
   -0.5f, 0.5f,-0.5f,   0,0,-1,   0,

    // Face +Z (frontal) id=1
   -0.5f,-0.5f, 0.5f,   0,0,1,    1,
    0.5f,-0.5f, 0.5f,   0,0,1,    1,
    0.5f, 0.5f, 0.5f,   0,0,1,    1,
   -0.5f, 0.5f, 0.5f,   0,0,1,    1,

    // Face -X (esquerda) id=2
   -0.5f,-0.5f,-0.5f,  -1,0,0,    2,
   -0.5f, 0.5f,-0.5f,  -1,0,0,    2,
   -0.5f, 0.5f, 0.5f,  -1,0,0,    2,
   -0.5f,-0.5f, 0.5f,  -1,0,0,    2,

    // Face +X (direita) id=3
    0.5f,-0.5f,-0.5f,   1,0,0,    3,
    0.5f, 0.5f,-0.5f,   1,0,0,    3,
    0.5f, 0.5f, 0.5f,   1,0,0,    3,
    0.5f,-0.5f, 0.5f,   1,0,0,    3,

    // Face -Y (baixo) id=4
   -0.5f,-0.5f,-0.5f,   0,-1,0,   4,
    0.5f,-0.5f,-0.5f,   0,-1,0,   4,
    0.5f,-0.5f, 0.5f,   0,-1,0,   4,
   -0.5f,-0.5f, 0.5f,   0,-1,0,   4,

    // Face +Y (cima) id=5
   -0.5f, 0.5f,-0.5f,   0,1,0,    5,
    0.5f, 0.5f,-0.5f,   0,1,0,    5,
    0.5f, 0.5f, 0.5f,   0,1,0,    5,
   -0.5f, 0.5f, 0.5f,   0,1,0,    5,
};


static unsigned int cubeIndices[] = {
    // Face -Z
    0,1,2,  2,3,0,
    // Face +Z
    4,5,6,  6,7,4,
    // Face -X
    8,9,10, 10,11,8,
    // Face +X
    12,13,14, 14,15,12,
    // Face -Y
    16,17,18, 18,19,16,
    // Face +Y
    20,21,22, 22,23,20
};


CubeRenderer::CubeRenderer()
: shader("shaders/cube.vert", "shaders/cube.frag")
{
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cubeIndices),cubeIndices,GL_STATIC_DRAW);

    // posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal (se não for usar, pode deixar quieto)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // faceId (como inteiro!)
    glVertexAttribIPointer(2, 1, GL_INT, 7 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

CubeRenderer::~CubeRenderer() {
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void CubeRenderer::setCubies(const std::vector<Cubie>& cubies) {
    this->cubies = cubies;
}

void CubeRenderer::updateCubieColors(const std::array<glm::vec3, 6>& colors) {
    float newVertices[8 * 6];
    // aqui você sobrescreve as cores de acordo com o array passado
    // cada face do cubo recebe a cor correta
    // (precisa definir uma convenção: qual vértice pertence a qual face)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newVertices), newVertices);
}

void CubeRenderer::draw(const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    for (const auto& c : cubies) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), c.position);
        shader.setMat4("model", model);

        // Setar todas as cores de uma vez
        glUniform3fv(glGetUniformLocation(shader.ID, "faceColors"), 6, glm::value_ptr(c.colors[0]));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}
