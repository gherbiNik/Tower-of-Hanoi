#include "omnidirectionalLight.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

ENG_API OmnidirectionalLight::OmnidirectionalLight() : Light()
{}

OmnidirectionalLight::~OmnidirectionalLight()
{}

float OmnidirectionalLight::getCutoff() const
{
    return cutoff;
}

void OmnidirectionalLight::render() 
{
    int lightId = GL_LIGHT0;

    glEnable(lightId);
    glLightfv(lightId, GL_AMBIENT, glm::value_ptr(getAmbient()));
    glLightfv(lightId, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
    glLightfv(lightId, GL_SPECULAR, glm::value_ptr(getSpecular()));
    glLightfv(lightId, GL_POSITION, glm::value_ptr(getPosition()));

    glLightf(lightId, GL_SPOT_CUTOFF, cutoff);

    // Attenuazione standard 
    glLightf(lightId, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(lightId, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(lightId, GL_QUADRATIC_ATTENUATION, 0.0f);
}