#include "texture.h"
#include <GL/freeglut.h>
// #include "FreeImage.h" // Necessario se implementi il caricamento file

Texture::Texture(unsigned int id, const std::string& name)
   : Object(id, name)
{
   // TODO: Implementare caricamento texture (glGenTextures, glTexImage2D, ecc.)
}

Texture::~Texture() {
   // TODO: glDeleteTextures(...);
}

void Texture::render() {
   // TODO: glBindTexture(GL_TEXTURE_2D, m_texId);
}