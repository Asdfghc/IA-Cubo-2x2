#pragma once
#include "shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <array>

struct Cubie {
    glm::vec3 position;               // posição do cubinho no espaço
    std::array<glm::vec3, 6> colors;  // uma cor por face: +X, -X, +Y, -Y, +Z, -Z
};

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();
    void draw(const glm::mat4& view, const glm::mat4& projection);
    void setCubies(const std::vector<Cubie>& cubies);
    void updateCubieColors(const std::array<glm::vec3, 6>& colors);

private:
    unsigned int VAO, VBO, EBO;
    Shader shader;
    std::vector<Cubie> cubies;
};
