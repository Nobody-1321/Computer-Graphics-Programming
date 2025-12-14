#pragma once
#include <vector>
#include <cstddef>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Torus {
public:
    Torus(float innerRadius, float outerRadius, std::size_t sectors = 32, std::size_t stacks = 16);

    const std::vector<unsigned int>& getIndices() const noexcept;
    const std::vector<glm::vec3>& getVertices() const noexcept;
    const std::vector<glm::vec3>& getNormals() const noexcept;
    const std::vector<glm::vec2>& getTexCoords() const noexcept;

    std::size_t getNumIndices() const noexcept;
    std::size_t getNumVertices() const noexcept;

private:
    void buildVertices();
    void buildIndices();

    float innerRadius;
    float outerRadius;
    std::size_t sectors;
    std::size_t stacks;

    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
};