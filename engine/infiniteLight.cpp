#include "infiniteLight.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

ENG_API InfiniteLight::InfiniteLight() : Light()
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

void InfiniteLight::render(){
	int lightId = GL_LIGHT0;

    glEnable(lightId);
    glLightfv(lightId, GL_AMBIENT, glm::value_ptr(getAmbient()));
    glLightfv(lightId, GL_DIFFUSE, glm::value_ptr(getDiffuse()));
    glLightfv(lightId, GL_SPECULAR, glm::value_ptr(getSpecular()));

    // La posizione con w=0.0 viene interpretata come direzione
    // (luce direzionale infinitamente distante)
    glLightfv(lightId, GL_POSITION, glm::value_ptr(getPosition()));
}
