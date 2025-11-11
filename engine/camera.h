#pragma once

#include "glm/glm.hpp"
#include "node.h"
/**
 * @brief Definisce il punto di vista e la proiezione per il rendering.
 */
class Camera : public Node{
public:
    Camera();

    float getNearPlane() const { return nearPlane; }
    void setNearPlane(float nearVal) { nearPlane = nearVal; }
    float getFarPlane() const { return farPlane; }
    void setFarPlane(float farVal) { farPlane = farVal; }

    glm::mat4 getInvCameraMatrix() const;
    glm::mat4 getViewMatrix() const { return viewMatrix; }
    void setViewMatrix(const glm::mat4& matrix) { viewMatrix = matrix; }
    glm::mat4 getProjMatrix() const { return projectionMatrix; }
    void setProjMatrix(const glm::mat4& matrix) { projectionMatrix = matrix; }

private:
    float nearPlane;
    float farPlane;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};