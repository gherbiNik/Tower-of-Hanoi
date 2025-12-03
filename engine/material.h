#pragma once
#include "object.h"
#include "texture.h"
#include "glm/glm.hpp"
#include "libConfig.h"
class ENG_API Material : public Object {
public:
	Material(const std::string& name); // Default constructor
	Material(const std::string& name, const glm::vec4& emission, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, float shininess, float transparency);
	~Material();
	// Getters
	const glm::vec4& getAmbient() const;
	const glm::vec4& getDiffuse() const;
	const glm::vec4& getSpecular() const;
	const glm::vec4& getEmissione() const;
	float getShininess() const;
	float getTransparency() const;
	Texture* getTexture();
	// Setters
	void setAmbient(const glm::vec4& ambient);
	void setDiffuse(const glm::vec4& diffuse);
	void setSpecular(const glm::vec4& specular);
	void setEmissione(const glm::vec4& emissione);
	void setShininess(float shininess);
	void setTransparency(float transparency);
	void setTexture(Texture* texture);
	void render() override; 
private:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emissione;
	float shininess;
	float transparency;
	Texture* texture;
};