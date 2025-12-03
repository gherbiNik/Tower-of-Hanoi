#pragma once
#include "light.h"
#include <glm/glm.hpp>

class ENG_API OmnidirectionalLight : public Light {
public:
    OmnidirectionalLight();
    OmnidirectionalLight(const std::string& name, const glm::mat4& matrix,
        const glm::vec3& ambient, const glm::vec3& diffuse,
        const glm::vec3& specular);
    ~OmnidirectionalLight();

    float getCutoff() const;

    void render() override;

private:
    float cutoff = 180.0f; // 180° = omnidirezionale
};