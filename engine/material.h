#pragma once
#include "object.h"
#include "texture.h"
#include "glm/glm.hpp"

class Material : public Object {
public:
   Material(unsigned int id, const std::string& name);  // Added constructor
   ~Material();

   // Getter
   const glm::vec4& getAmbient() const;
   const glm::vec4& getDiffuse() const;
   const glm::vec4& getSpecular() const;
   const glm::vec4& getEmissione() const;
   float getShininess() const;
   Texture* getTexture();

   // Setter
   void setAmbient(const glm::vec4& ambient);
   void setDiffuse(const glm::vec4& diffuse);
   void setSpecular(const glm::vec4& specular);
   void setEmissione(const glm::vec4& emissione);
   void setShininess(float shininess);
   void setTexture(Texture* texture);

   void render() override;  // Must implement

private:
   glm::vec4 ambient;
   glm::vec4 diffuse;
   glm::vec4 specular;
   glm::vec4 emissione;
   float shininess;
   Texture* texture;
};