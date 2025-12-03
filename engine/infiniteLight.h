#pragma once
#include "light.h"
#include <glm/glm.hpp>

class ENG_API InfiniteLight : public Light {
public:
    InfiniteLight();
    InfiniteLight(const std::string& name, const glm::mat4& matrix,
        const glm::vec3& ambient, const glm::vec3& diffuse,
        const glm::vec3& specular, const glm::vec3& direction);
    ~InfiniteLight();

    void setDirection(const glm::vec3& dir);
    glm::vec3 getDirection() const;

    void render() override;
};