#pragma once

#include <glm/glm.hpp>
#include <vector>

class Sphere {
private:
    float radius;
    unsigned int sectorCount;
    unsigned int stackCount;
   
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    void buildVertices();
    void buildIndices();
    
    // Cambiado a static porque no depende del estado de la instancia
    static float toRadians(float degrees); 

public:
    // Unificamos a unsigned int para los conteos
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);
    // Cambiado a float el radio para consistencia, con valores por defecto
    Sphere(float radius = 1.0f); 

    size_t getNumVertices() const;
    size_t getNumIndices() const;
    
    // Retornos por referencia constante para evitar copias masivas
    const std::vector<unsigned int>& getIndices() const;
    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<glm::vec2>& getTexCoords() const;
};