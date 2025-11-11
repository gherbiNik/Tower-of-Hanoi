#pragma once

#include "Object.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @brief Rappresenta un nodo nel grafo della scena, gestisce trasformazioni e gerarchia.
 */
class Node : public Object {
public:
    Node(unsigned int id, const std::string& name);

    glm::mat4 getM() const { return transformationMatrix; }
    void setM(const glm::mat4& newMatrix) { transformationMatrix = newMatrix; }

    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& factor);
    void translate(const glm::vec3& translation);

    glm::mat4 getWorldFinalMatrix() const;

    void render() override;

    Node* getParent() const { return parent; }
    void setParent(Node* newParent) { parent = newParent; }

    void addChild(Node* child);
    Node* getChild(int index) const;
    unsigned int getNumChildren() const { return (unsigned int)children.size(); }

    Node* findByName(const std::string& nodeName);

private:
    glm::mat4 transformationMatrix;
    std::vector<Node*> children;
    Node* parent;
};