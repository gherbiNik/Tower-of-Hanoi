#pragma once
#include "node.h"

class Light : public Node
{
public:
	Light();
	virtual ~Light() = 0;

	// Getters
	const glm::vec4& getAmbient() const;
	const glm::vec4& getDiffuse() const;
	const glm::vec4& getSpecular() const;
	const glm::vec4& getPosition() const;

	// Setters
	void setAmbient(const glm::vec4& ambient);
	void setDiffuse(const glm::vec4& diffuse);
	void setSpecular(const glm::vec4& specular);
	void setPosition(const glm::vec4& position);

	void render() override;  // Must implement pure virtual

private:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
};