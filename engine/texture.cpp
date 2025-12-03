#define FREEIMAGE_LIB 
#include "texture.h"
#include <GL/freeglut.h>
#include "FreeImage.h"

Texture::Texture(const std::string& name, const std::string& filepath)
	: Object(name), m_filepath(filepath)
{
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(m_filepath.c_str(), 0), m_filepath.c_str());
	if (bitmap) {
		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		if (pImage) {
			int nWidth = FreeImage_GetWidth(pImage);
			int nHeight = FreeImage_GetHeight(pImage);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
			FreeImage_Unload(pImage);
		}
		FreeImage_Unload(bitmap);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::~Texture() {
	glDeleteTextures(1, &m_texId);
}
void Texture::render() {
	glBindTexture(GL_TEXTURE_2D, m_texId);
}