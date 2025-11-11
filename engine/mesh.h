#pragma once

#include "node.h"
#include "material.h"
#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Rappresenta un oggetto geometrico (una maglia) con vertici.
 */
class Mesh : public Node {
public:
    Mesh(unsigned int id, const std::string& name);

    // Getter
    const std::vector<float>& getVertices() const;
    Material* getMaterial() const;

    // Setter
    void setVertices(const std::vector<float>& vertices);
    void setMaterial(Material* material);

    void render() override;

private:
    std::vector<float> vertices;
    Material* material;

};