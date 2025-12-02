#include "infiniteLight.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>




InfiniteLight::InfiniteLight() : Light()
{
   setPosition(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
}

InfiniteLight::~InfiniteLight()
{
}
void InfiniteLight::setDirection(const glm::vec3& dir) {
    // w=0.0 => luce direzionale    
    setPosition(glm::vec4(dir.x, dir.y, dir.z, 0.0f));
}
glm::vec3 InfiniteLight::getDirection() const {
    glm::vec4 pos = getPosition();
    return glm::vec3(pos.x, pos.y, pos.z);
}

void InfiniteLight::render() {
   if (lightContextID < 0) return; // <--- Usa lightContextID

   glEnable(lightContextID);
   glLightfv(lightContextID, GL_AMBIENT, glm::value_ptr(getAmbient()));
   glLightfv(lightContextID, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
   glLightfv(lightContextID, GL_SPECULAR, glm::value_ptr(getSpecular()));

   // Luce direzionale: w=0 nel vettore posizione (gestito dal costruttore o setter)
   glLightfv(lightContextID, GL_POSITION, glm::value_ptr(getPosition()));
}
