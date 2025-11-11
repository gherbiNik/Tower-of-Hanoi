#pragma once

#include "Node.h"
#include <vector>
#include <glm/glm.hpp>

/**
 * @brief Rappresenta un oggetto geometrico (una maglia) con vertici.
 */
class Mesh : public Node {
public:
    Mesh(unsigned int id, const std::string& name);

    void setVertices(const std::vector<float>& newVertices);
    const std::vector<float>& getVertices() const { return vertices; }

    void render() override;

private:
    std::vector<float> vertices;
};