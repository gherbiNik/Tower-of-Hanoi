#pragma once
#include "object.h"
#include "libConfig.h"

class ENG_API Texture : public Object {
public:
   Texture(const std::string& name);  // Added constructor
   ~Texture();
   void render() override;
private:
};