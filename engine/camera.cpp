#include "camera.h"
#include <glm/gtc/matrix_transform.hpp> 

Camera::Camera(const std::string& name) : Node(name), projectionMatrix(glm::mat4(1.0f)) {}

Camera::~Camera() {}

const glm::mat4& Camera::getProjectionMatrix() const {
   return projectionMatrix;
}

void Camera::setProjectionMatrix(const glm::mat4& projectionMatrix) {
   this->projectionMatrix = projectionMatrix;
}

glm::mat4 Camera::getInvCameraMatrix() const {
   // La View Matrix è l'inversa della posizione/rotazione della camera nel mondo
   return glm::inverse(this->getWorldFinalMatrix());
}