#include "spotLight.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

ENG_API SpotLight::SpotLight() : Light(), cutoff(45.0f)
{
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

void SpotLight::render()
{


    glEnable(GL_LIGHT0 + lightID);
    glLightfv(GL_LIGHT0 + lightID, GL_AMBIENT, glm::value_ptr(getAmbient()));
    glLightfv(GL_LIGHT0 + lightID, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
    glLightfv(GL_LIGHT0 + lightID, GL_SPECULAR, glm::value_ptr(getSpecular()));
    glLightfv(GL_LIGHT0 + lightID, GL_POSITION, glm::value_ptr(getPosition()));

    glLightf(GL_LIGHT0 + lightID, GL_SPOT_CUTOFF, cutoff);
    
    // Direzione dello spot (di default verso -Z)
    glm::vec3 spotDirection(0.0f, 0.0f, -1.0f);
    glLightfv(GL_LIGHT0 + lightID, GL_SPOT_DIRECTION, glm::value_ptr(spotDirection));
    
    // Concentrazione del fascio (Esponente)
    glLightf(GL_LIGHT0 + lightID, GL_SPOT_EXPONENT, 0.0f);
    
    // Attenuazione
    glLightf(GL_LIGHT0 + lightID, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0 + lightID, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT0 + lightID, GL_QUADRATIC_ATTENUATION, 0.0f);
}