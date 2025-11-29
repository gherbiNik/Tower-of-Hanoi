#include "light.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

ENG_API Light::Light() : Node(0, "Light"),
ambient(0.0f), diffuse(1.0f), specular(1.0f), position(0.0f, 0.0f, 0.0f, 1.0f)
{
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

void Light::render() {
   // Esempio semplificato: usa sempre GL_LIGHT0. 
   // In un motore avanzato useresti un LightManager per assegnare ID dinamici.
   int lightId = GL_LIGHT0;

   glEnable(lightId);
   glLightfv(lightId, GL_AMBIENT, glm::value_ptr(ambient));
   glLightfv(lightId, GL_DIFFUSE, glm::value_ptr(diffuse));
   glLightfv(lightId, GL_SPECULAR, glm::value_ptr(specular));

   // La posizione viene trasformata dalla matrice ModelView corrente
   glLightfv(lightId, GL_POSITION, glm::value_ptr(position));
}