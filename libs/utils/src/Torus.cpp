#include "../include/Torus.hpp"
#include <cmath>

Torus::Torus(float innerRadius_, float outerRadius_, std::size_t sectors_, std::size_t stacks_)
    : innerRadius(innerRadius_), outerRadius(outerRadius_), sectors(sectors_), stacks(stacks_)
{
    if (sectors == 0 || stacks == 0) return;

    // reservar para evitar realocaciones
    const std::size_t vertsPerRow = sectors + 1;
    const std::size_t totalVerts = (stacks + 1) * vertsPerRow;
    vertices.reserve(totalVerts);
    normals.reserve(totalVerts);
    texCoords.reserve(totalVerts);
    indices.reserve(stacks * sectors * 6);

    buildVertices();
    buildIndices();
}

void Torus::buildVertices() {
    // precomputar sin/cos para sectores y stacks
    const float TWO_PI = 2.0f * static_cast<float>(M_PI);
    const float sectorStep = TWO_PI / static_cast<float>(sectors);
    const float stackStep = TWO_PI / static_cast<float>(stacks);

    std::vector<float> cosSector(sectors + 1), sinSector(sectors + 1);
    for (std::size_t j = 0; j <= sectors; ++j) {
        float a = j * sectorStep;
        cosSector[j] = std::cos(a);
        sinSector[j] = std::sin(a);
    }

    std::vector<float> cosStack(stacks + 1), sinStack(stacks + 1);
    for (std::size_t i = 0; i <= stacks; ++i) {
        float a = i * stackStep;
        cosStack[i] = std::cos(a);
        sinStack[i] = std::sin(a);
    }

    for (std::size_t i = 0; i <= stacks; ++i) {
        for (std::size_t j = 0; j <= sectors; ++j) {
            float cs = cosSector[j];
            float ss = sinSector[j];
            float ck = cosStack[i];
            float sk = sinStack[i];

            float x = (outerRadius + innerRadius * cs) * ck;
            float y = (outerRadius + innerRadius * cs) * sk;
            float z = innerRadius * ss;

            vertices.emplace_back(x, y, z);

            // normal aproximada (no necesita normalizar si innerRadius>0 y transformaciones uniformes)
            float nx = ck * cs;
            float ny = sk * cs;
            float nz = ss;
            normals.emplace_back(nx, ny, nz);

            float s = static_cast<float>(j) / static_cast<float>(sectors);
            float t = static_cast<float>(i) / static_cast<float>(stacks);
            texCoords.emplace_back(s, t);
        }
    }
}

void Torus::buildIndices()
{
    const std::size_t vertsPerRow = sectors + 1;
    for (std::size_t i = 0; i < stacks; ++i) {
        std::size_t k1 = i * vertsPerRow;
        std::size_t k2 = k1 + vertsPerRow;

        for (std::size_t j = 0; j < sectors; ++j, ++k1, ++k2) {
            indices.push_back(static_cast<unsigned int>(k1));
            indices.push_back(static_cast<unsigned int>(k2));
            indices.push_back(static_cast<unsigned int>(k1 + 1));

            indices.push_back(static_cast<unsigned int>(k1 + 1));
            indices.push_back(static_cast<unsigned int>(k2));
            indices.push_back(static_cast<unsigned int>(k2 + 1));
        }
    }
}

const std::vector<unsigned int>& Torus::getIndices() const noexcept { return indices; }
const std::vector<glm::vec3>& Torus::getVertices() const noexcept { return vertices; }
const std::vector<glm::vec3>& Torus::getNormals() const noexcept { return normals; }
const std::vector<glm::vec2>& Torus::getTexCoords() const noexcept { return texCoords; }

std::size_t Torus::getNumIndices() const noexcept { return indices.size(); }
std::size_t Torus::getNumVertices() const noexcept { return vertices.size(); }