#include "material.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp> // Per glm::value_ptr

Material::Material(const std::string& name)
	: Object(name), shininess(1.0f), transparency(0.0f), texture(nullptr),
	ambient(0.2f, 0.2f, 0.2f, 1.0f), diffuse(0.8f, 0.8f, 0.8f, 1.0f), specular(0.0f, 0.0f, 0.0f, 1.0f), emissione(0.0f, 0.0f, 0.0f, 1.0f)
{
}
Material::Material(const std::string& name, const glm::vec4& emission, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, float shininess, float transparency)
	: Object(name), emissione(emission), ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), transparency(transparency), texture(nullptr)
{
}

Material::~Material() {}

// Getters
const glm::vec4& Material::getAmbient() const { return ambient; }
const glm::vec4& Material::getDiffuse() const { return diffuse; }
const glm::vec4& Material::getSpecular() const { return specular; }
const glm::vec4& Material::getEmissione() const { return emissione; }
float Material::getShininess() const { return shininess; }
float Material::getTransparency() const { return transparency; }
Texture* Material::getTexture() { return texture; }

// Setters
void Material::setAmbient(const glm::vec4& v) { ambient = v; }
void Material::setDiffuse(const glm::vec4& v) { diffuse = v; }
void Material::setSpecular(const glm::vec4& v) { specular = v; }
void Material::setEmissione(const glm::vec4& v) { emissione = v; }
void Material::setShininess(float v) { shininess = v; }
void Material::setTransparency(float v) { transparency = v; }
void Material::setTexture(Texture* t) { texture = t; }

void Material::render() {
	float opacity = 1.0f - transparency;
	glm::vec4 amb = ambient;
	amb.a *= opacity;
	glm::vec4 dif = diffuse;
	dif.a *= opacity;
	glm::vec4 spec = specular;
	spec.a *= opacity;
	// Imposta parametri di shading Phong/Blinn-Phong
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(amb));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(dif));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(spec));
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