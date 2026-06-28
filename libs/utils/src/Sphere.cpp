#include "../include/Sphere.hpp"
#include <cmath> // Para M_PI, cosf, sinf

// Usando listas de inicialización
Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount) 
    : radius(radius), sectorCount(sectorCount), stackCount(stackCount) 
{
    buildVertices();
    buildIndices();
}

Sphere::Sphere(float radius) 
    : radius(radius), sectorCount(36), stackCount(18) 
{
    buildVertices();
    buildIndices();
}

void Sphere::buildVertices() {
    // 1. Reservar memoria de golpe para evitar realocaciones dinámicas
    unsigned int numVertices = (stackCount + 1) * (sectorCount + 1);
    vertices.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);

    float x, y, z, xy;
    float lengthInv = 1.0f / radius;
    float s, t;

    // Nota: M_PI a veces requiere _USE_MATH_DEFINES en Windows, o puedes usar glm::pi<float>()
    float sectorStep = 2.0f * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    // Cambiado los iteradores a unsigned int para coincidir con stackCount y sectorCount
    for(unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2.0f - i * stackStep;
        xy = radius * std::cos(stackAngle);
        z = radius * std::sin(stackAngle);

        for(unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * std::cos(sectorAngle);
            y = xy * std::sin(sectorAngle);
            vertices.emplace_back(x, y, z); // emplace_back suele ser más limpio que vec3(...)
            
            normals.emplace_back(x * lengthInv, y * lengthInv, z * lengthInv);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.emplace_back(s, t);
        }
    }
}

void Sphere::buildIndices() {
    // Reservar memoria estimada para los índices
    // Cada sector de cada stack (menos los extremos si se optimiza, pero aproximado) tiene 2 triángulos (6 índices)
    indices.reserve(stackCount * sectorCount * 6);

    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // Ya no hacen falta los static_cast innecesarios porque todo es unsigned int
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

float Sphere::toRadians(float degrees) {
    return (degrees * 2.0f * M_PI) / 360.0f; 
}

// Métodos const y con tipos correctos
size_t Sphere::getNumVertices() const {
    return vertices.size();
}

size_t Sphere::getNumIndices() const {
    return indices.size();
}

const std::vector<unsigned int>& Sphere::getIndices() const {
    return indices;
}

const std::vector<glm::vec3>& Sphere::getVertices() const {
    return vertices;
}

const std::vector<glm::vec2>& Sphere::getTexCoords() const {
    return texCoords;
}

const std::vector<glm::vec3>& Sphere::getNormals() const {
    return normals;
}

