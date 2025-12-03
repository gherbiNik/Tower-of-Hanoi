#include "spotLight.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

SpotLight::SpotLight() : Light(), cutoff(45.0f), spotExponent(0.0f)
{
}

SpotLight::SpotLight(const std::string& name, const glm::mat4& matrix,
    const glm::vec3& ambient, const glm::vec3& diffuse,
    const glm::vec3& specular, const glm::vec3& dir)
    : Light(name, matrix), cutoff(45.0f), spotExponent(0.0f)
{
    setAmbient(glm::vec4(ambient, 1.0f));
    setDiffuse(glm::vec4(diffuse, 1.0f));
    setSpecular(glm::vec4(specular, 1.0f));

    // Estrai la posizione dalla matrice di trasformazione
    glm::vec3 position(matrix[3][0], matrix[3][1], matrix[3][2]);
    setPosition(glm::vec4(position, 1.0f)); // w=1.0 per luce posizionale

    setDirection(dir);
}

SpotLight::~SpotLight()
{
}

float SpotLight::getCutoff() const
{
    return cutoff;
}

void SpotLight::setCutoff(float value)
{
    cutoff = value;
}

void SpotLight::setDirection(const glm::vec3& dir)
{
    direction = glm::normalize(dir);
}

glm::vec3 SpotLight::getDirection() const
{
    return direction;
}

void SpotLight::setSpotExponent(float exp)
{
    spotExponent = exp;
}

float SpotLight::getSpotExponent() const
{
    return spotExponent;
}

void SpotLight::render()
{
    if (lightContextID < 0) return;

    glEnable(lightContextID);
    glLightfv(lightContextID, GL_AMBIENT, glm::value_ptr(getAmbient()));
    glLightfv(lightContextID, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
    glLightfv(lightContextID, GL_SPECULAR, glm::value_ptr(getSpecular()));
    glLightfv(lightContextID, GL_POSITION, glm::value_ptr(getPosition()));

    glLightf(lightContextID, GL_SPOT_CUTOFF, cutoff);

    // Direzione dello spot
    glLightfv(lightContextID, GL_SPOT_DIRECTION, glm::value_ptr(direction));

    glLightf(lightContextID, GL_SPOT_EXPONENT, spotExponent);

    // Attenuazione
    glLightf(lightContextID, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(lightContextID, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(lightContextID, GL_QUADRATIC_ATTENUATION, 0.0f);
}