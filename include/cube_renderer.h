#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "shader.h"

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();
    void updateColors(const std::vector<glm::vec3>& colors);
    void draw(const glm::mat4& view, const glm::mat4& projection);
private:
    GLuint VAO, VBO, EBO;
    Shader shader;
    std::vector<glm::vec3> currentColors;
};

#endif
