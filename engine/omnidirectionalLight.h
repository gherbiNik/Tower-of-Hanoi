#pragma once
#include "light.h"
#include "libConfig.h"

class ENG_API OmnidirectionalLight : public Light
{
public:
	OmnidirectionalLight();
	~OmnidirectionalLight();
	// Getter
	float getCutoff() const;

private:
	float cutoff = 180.0f;
};
