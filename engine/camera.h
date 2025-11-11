#pragma once

#include "glm/glm.hpp"
#include "node.h"

/**
 * @brief Definisce il punto di vista e la proiezione per il rendering.
 */
class Camera : public Node {
public:
   Camera(unsigned int id, const std::string& name);  // Added parameters
   ~Camera();

   // Getter
   const glm::mat4& getProjectionMatrix() const;
   glm::mat4 getInvCameraMatrix() const;

   // Setter
   void setProjectionMatrix(const glm::mat4& projectionMatrix);

private:
   glm::mat4 projectionMatrix;
};