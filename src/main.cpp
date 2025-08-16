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

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.1f,0.1f,0.1f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Atualiza estado do cubo
        //state.applyMove(Move::U);
        renderer.updateColors({
            glm::vec3(1,0,0),   // Red
            glm::vec3(0,1,0),   // Green
            glm::vec3(0,0,1),   // Blue
            glm::vec3(1,1,0),   // Yellow
            glm::vec3(1,0,1),   // Magenta
            glm::vec3(0,1,1),   // Cyan
            glm::vec3(1,1,1),   // White
            glm::vec3(0,0,0)    // Black
        });

        glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0,0,-3));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);

        renderer.draw(view,projection);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
