#pragma once
#include "node.h"
#include "material.h"
#include <vector>
#include <glm/glm.hpp>
#include "libConfig.h"

/**
@brief Rappresenta un oggetto geometrico (una maglia) con vertici.
*/
class ENG_API Mesh : public Node {
public:
    Mesh(const std::string& name);
    Mesh(const std::string& name, glm::mat4 matrix, unsigned int faces, unsigned int vertices, Material* material);

    // Getters
    const std::vector<glm::vec3>& get_all_vertices() const;
    const std::vector<glm::vec3>& get_all_normals() const;
    const std::vector<glm::vec2>& get_all_texture_coords() const;
    const std::vector<std::vector<unsigned int>>& get_face_vertices() const;
    Material* getMaterial() const;

    // Setters
    void set_all_vertices(const std::vector<glm::vec3>& vertices);
    void set_all_normals(const std::vector<glm::vec3>& normals);
    void set_all_texture_coords(const std::vector<glm::vec2>& textureCoords);
    void set_face_vertices(const std::vector<std::vector<unsigned int>>& faces);
    void setMaterial(Material* material);
    void render() override;

private:
    std::vector<glm::vec3> all_vertices;
    std::vector<glm::vec3> all_normals;
    std::vector<glm::vec2> all_texture_coords;
    std::vector<std::vector<unsigned int>> face_vertices;
    unsigned int numFaces;
    unsigned int numVertices;
    Material* material;
    glm::mat4 matrix;
};