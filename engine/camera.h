#pragma once

#include "glm/glm.hpp"
#include "node.h"
#include "libConfig.h"

/**
 * @brief Definisce il punto di vista e la proiezione per il rendering.
 */
class ENG_API Camera : public Node {
public:
   Camera(const std::string& name);  // Added parameters
   ~Camera();

   // Getter
   const glm::mat4& getProjectionMatrix() const;
   glm::mat4 getInvCameraMatrix() const;

   // Setterç
   void setProjectionMatrix(const glm::mat4& projectionMatrix);

private:
   glm::mat4 projectionMatrix;
};