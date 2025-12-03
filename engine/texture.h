#pragma once
#include "object.h"
#include "libConfig.h"
#include <string>

class ENG_API Texture : public Object {
public:
	Texture(const std::string& name, const std::string& filepath); // Updated constructor to take filepath
	~Texture();
	void render() override;
private:
	std::string m_filepath;
	unsigned int m_texId;
};