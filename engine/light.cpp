#include "light.h"
#include <stdexcept>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

unsigned int ENG_API Light::m_helper_light_id = 0;

int ENG_API Light::m_max_lights = 0;

ENG_API Light::Light() : Node("Light"), lightID{ m_helper_light_id }, ambient(0.0f), diffuse(1.0f), specular(1.0f), position(0.0f, 0.0f, 0.0f, 1.0f)
{

   if (m_helper_light_id == 0) {
      glGetIntegerv(GL_MAX_LIGHTS, &m_max_lights);
   }
   else if (m_helper_light_id > size_t(m_max_lights - 1))
      throw std::runtime_error("Maximum number of possible lights reached!");
   m_helper_light_id++;
}

Light::~Light() {}

// Getter & Setter
const glm::vec4& Light::getAmbient() const { return ambient; }
const glm::vec4& Light::getDiffuse() const { return diffuse; }
const glm::vec4& Light::getSpecular() const { return specular; }
const glm::vec4& Light::getPosition() const { return position; }

void Light::setAmbient(const glm::vec4& v) { ambient = v; }
void Light::setDiffuse(const glm::vec4& v) { diffuse = v; }
void Light::setSpecular(const glm::vec4& v) { specular = v; }
void Light::setPosition(const glm::vec4& v) { position = v; }
void Light::setLightID(int id) {
   this->lightID = id;
}
