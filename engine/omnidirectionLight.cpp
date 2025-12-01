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


    glEnable(GL_LIGHT0 + lightID);
    glLightfv(GL_LIGHT0 + lightID, GL_AMBIENT, glm::value_ptr(getAmbient()));
    glLightfv(GL_LIGHT0 + lightID, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
    glLightfv(GL_LIGHT0 + lightID, GL_SPECULAR, glm::value_ptr(getSpecular()));
    glLightfv(GL_LIGHT0 + lightID, GL_POSITION, glm::value_ptr(getPosition()));

    glLightf(GL_LIGHT0 + lightID, GL_SPOT_CUTOFF, cutoff);

    // Attenuazione standard 
    glLightf(GL_LIGHT0 + lightID, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0 + lightID, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT0 + lightID, GL_QUADRATIC_ATTENUATION, 0.0f);
}