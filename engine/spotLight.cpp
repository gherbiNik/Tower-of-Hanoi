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
   if (lightContextID < 0) return; // <--- Usa lightContextID

   glEnable(lightContextID);
   glLightfv(lightContextID, GL_AMBIENT, glm::value_ptr(getAmbient()));
   glLightfv(lightContextID, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
   glLightfv(lightContextID, GL_SPECULAR, glm::value_ptr(getSpecular()));
   glLightfv(lightContextID, GL_POSITION, glm::value_ptr(getPosition()));

   glLightf(lightContextID, GL_SPOT_CUTOFF, cutoff);

   // Direzione dello spot (di default verso -Z locale, o custom se implementato)
   // Nota: in fixed pipeline la direzione dello spot va settata con GL_SPOT_DIRECTION
   glm::vec3 spotDirection(0.0f, 0.0f, -1.0f);
   glLightfv(lightContextID, GL_SPOT_DIRECTION, glm::value_ptr(spotDirection));

   glLightf(lightContextID, GL_SPOT_EXPONENT, 0.0f); // o altro valore

   // Attenuazione
   glLightf(lightContextID, GL_CONSTANT_ATTENUATION, 1.0f);
   glLightf(lightContextID, GL_LINEAR_ATTENUATION, 0.0f);
   glLightf(lightContextID, GL_QUADRATIC_ATTENUATION, 0.0f);
}