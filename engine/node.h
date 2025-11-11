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
    virtual ~Node() = default;

    // Transformation matrix accessors
    glm::mat4 getM() const;
    void setM(const glm::mat4& newMatrix);

    // Transformation methods
    void rotate(float angle, const glm::vec3& axis);
    void scale(const glm::vec3& factor);
    void translate(const glm::vec3& translation);

    // World matrix computation
    glm::mat4 getWorldFinalMatrix() const;

    // Override render from Object
    void render() override;

    // Parent-child hierarchy
    Node* getParent() const;
    void setParent(Node* newParent);

    void addChild(Node* child);
    void removeChild(Node* child);
    Node* getChild(int index) const;
    unsigned int getNumChildren() const;

    // Tree search
    Node* findByName(const std::string& nodeName);

private:
    glm::mat4 transformationMatrix;
    std::vector<Node*> children;
    Node* parent;
};