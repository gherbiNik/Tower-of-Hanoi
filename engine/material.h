#pragma once
#include "object.h"
#include "texture.h"
#include "glm/glm.hpp"
#include "libConfig.h"
class ENG_API Material : public Object {
public:
	Material(const std::string& name, const glm::vec3& emission, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, float transparency);
	~Material();
	// Getters
	const glm::vec3& getAmbient() const;
	const glm::vec3& getDiffuse() const;
	const glm::vec3& getSpecular() const;
	const glm::vec3& getEmissione() const;
	float getShininess() const;
	float getTransparency() const;
	Texture* getTexture();
	// Setters
	void setAmbient(const glm::vec3& ambient);
	void setDiffuse(const glm::vec3& diffuse);
	void setSpecular(const glm::vec3& specular);
	void setEmissione(const glm::vec3& emissione);
	void setShininess(float shininess);
	void setTransparency(float transparency);
	void setTexture(Texture* texture);
	void render() override; 
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissione;
	float shininess;
	float transparency;
	Texture* texture;
};