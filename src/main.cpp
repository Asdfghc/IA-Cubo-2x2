#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "cube_renderer.h"
#include <iostream>

int main() {
    if(!glfwInit()){ std::cerr<<"Falha em GLFW\n"; return -1;}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800,600,"Cubo 3D",NULL,NULL);
    if(!window){ std::cerr<<"Falha ao criar janela\n"; glfwTerminate(); return -1;}
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){ std::cerr<<"Falha em GLEW\n"; return -1;}

    glEnable(GL_DEPTH_TEST);

    CubeRenderer renderer;
    //CubeState state;

    // Cria os cubies do cubo 2x2 (8 peças)
    std::vector<Cubie> cubies;
    float offset = 0.55f; // distância entre os cubies

    for (int x = -1; x <= 1; x += 2) {
        for (int y = -1; y <= 1; y += 2) {
            for (int z = -1; z <= 1; z += 2) {
                Cubie c;
                c.position = glm::vec3(x * offset, y * offset, z * offset);
                c.colors = {
                    glm::vec3(1.0f,0.0f,0.0f),   // face 0: vermelho
                    glm::vec3(0.0f,1.0f,0.0f),   // face 1: verde
                    glm::vec3(0.0f,0.0f,1.0f),   // face 2: azul
                    glm::vec3(1.0f,1.0f,0.0f),   // face 3: amarelo
                    glm::vec3(1.0f,0.0f,1.0f),   // face 4: magenta
                    glm::vec3(0.0f,1.0f,1.0f)    // face 5: ciano
                };
                cubies.push_back(c);
            }
        }
    }
    renderer.setCubies(cubies);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


         glm::mat4 view = glm::lookAt(
            glm::vec3(3, 3, 5),  // posição da câmera
            glm::vec3(0, 0, 0),  // olha para o centro
            glm::vec3(0, 1, 0)   // up
        );
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            800.0f / 600.0f,
            0.1f,
            100.0f
        );

        renderer.draw(view,projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
