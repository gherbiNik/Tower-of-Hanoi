#pragma once
#include "light.h"
#include <glm/glm.hpp>

class ENG_API SpotLight : public Light {
public:
    SpotLight();
    SpotLight(const std::string& name, const glm::mat4& matrix,
       const glm::vec3& ambient, const glm::vec3& diffuse,
       const glm::vec3& specular, const glm::vec3& direction,
       float cutoff, float exponent);
    ~SpotLight();

    float getCutoff() const;
    void setCutoff(float value);

    void setDirection(const glm::vec3& dir);
    glm::vec3 getDirection() const;

    void setSpotExponent(float exp);
    float getSpotExponent() const;

    void render() override;

private:
    float cutoff;
    float spotExponent = 0.0f;
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
};