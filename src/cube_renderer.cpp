#include "cube_renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

float cubeVertices[] = {
    // pos              // cor
   -0.5f,-0.5f,-0.5f, 1,0,0,
    0.5f,-0.5f,-0.5f, 0,1,0,
    0.5f, 0.5f,-0.5f, 0,0,1,
   -0.5f, 0.5f,-0.5f, 1,1,0,
   -0.5f,-0.5f, 0.5f, 1,0,1,
    0.5f,-0.5f, 0.5f, 0,1,1,
    0.5f, 0.5f, 0.5f, 1,1,1,
   -0.5f, 0.5f, 0.5f, 0,0,0
};

unsigned int cubeIndices[] = {
0,1,2,2,3,0,
4,5,6,6,7,4,
0,1,5,5,4,0,
2,3,7,7,6,2,
0,3,7,7,4,0,
1,2,6,6,5,1
};

CubeRenderer::CubeRenderer(): shader("shaders/cube.vert","shaders/cube.frag") {
    currentColors.resize(8);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cubeIndices),cubeIndices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}

CubeRenderer::~CubeRenderer() {
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void CubeRenderer::updateColors(const std::vector<glm::vec3>& colors) {
    currentColors = colors;
    // Aqui você poderia atualizar VBO de cores se quiser cores dinâmicas
}

void CubeRenderer::draw(const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    glm::mat4 model = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime(),glm::vec3(0,1,0));
    shader.setMat4("model",model);
    shader.setMat4("view",view);
    shader.setMat4("projection",projection);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
}
