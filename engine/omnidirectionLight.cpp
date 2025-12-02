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
   // Se l'ID non è valido, esci
   if (lightContextID < 0) return; // <--- Usa lightContextID

   glEnable(lightContextID); // <--- Usa lightContextID ovunque
   glLightfv(lightContextID, GL_AMBIENT, glm::value_ptr(getAmbient()));
   glLightfv(lightContextID, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
   glLightfv(lightContextID, GL_SPECULAR, glm::value_ptr(getSpecular()));

   // W = 1.0f per luci posizionali
   glLightfv(lightContextID, GL_POSITION, glm::value_ptr(getPosition()));

   glLightf(lightContextID, GL_SPOT_CUTOFF, cutoff); // 180.0 per omni

   // Attenuazione standard 
   glLightf(lightContextID, GL_CONSTANT_ATTENUATION, 1.0f);
   glLightf(lightContextID, GL_LINEAR_ATTENUATION, 0.0f);
   glLightf(lightContextID, GL_QUADRATIC_ATTENUATION, 0.0f);
}