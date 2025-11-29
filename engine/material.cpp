#include "material.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Per glm::value_ptr

Material::Material(unsigned int id, const std::string& name)
   : Object(id, name), shininess(1.0f), texture(nullptr),
   ambient(0.2f), diffuse(0.8f), specular(0.0f), emissione(0.0f)
{
}

Material::~Material() {}

// Getter
const glm::vec4& Material::getAmbient() const { return ambient; }
const glm::vec4& Material::getDiffuse() const { return diffuse; }
const glm::vec4& Material::getSpecular() const { return specular; }
const glm::vec4& Material::getEmissione() const { return emissione; }
float Material::getShininess() const { return shininess; }
Texture* Material::getTexture() { return texture; }

// Setter
void Material::setAmbient(const glm::vec4& v) { ambient = v; }
void Material::setDiffuse(const glm::vec4& v) { diffuse = v; }
void Material::setSpecular(const glm::vec4& v) { specular = v; }
void Material::setEmissione(const glm::vec4& v) { emissione = v; }
void Material::setShininess(float v) { shininess = v; }
void Material::setTexture(Texture* t) { texture = t; }

void Material::render() {
   // Imposta parametri di shading Phong/Blinn-Phong
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient));
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse));
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular));
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(emissione));
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

   // Gestione Texture
   if (texture) {
      glEnable(GL_TEXTURE_2D);
      texture->render(); // Effettua il bind
   }
   else {
      glDisable(GL_TEXTURE_2D);
   }
}