#pragma once
#include "object.h"

class Texture : public Object {
public:
   Texture(unsigned int id, const std::string& name);  // Added constructor
   ~Texture();
   void render() override;
private:
};