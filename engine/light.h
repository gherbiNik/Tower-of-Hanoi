#pragma once
#include "node.h"
#include "libConfig.h"

class ENG_API Light : public Node
{
public:
   Light();
   virtual ~Light() = 0; // Puro virtuale

   // Getters & Setters
   const glm::vec4& getAmbient() const;
   const glm::vec4& getDiffuse() const;
   const glm::vec4& getSpecular() const;
   const glm::vec4& getPosition() const;

   void setAmbient(const glm::vec4& ambient);
   void setDiffuse(const glm::vec4& diffuse);
   void setSpecular(const glm::vec4& specular);
   void setPosition(const glm::vec4& position);

   // Metodo usato dalla List per assegnare l'ID hardware (GL_LIGHTi)
   void setLightID(int id);

   virtual void render() override;

protected:
   glm::vec4 ambient;
   glm::vec4 diffuse;
   glm::vec4 specular;
   glm::vec4 position;

   int lightContextID; // ID assegnato dinamicamente (-1 se spenta/non renderizzata)
};