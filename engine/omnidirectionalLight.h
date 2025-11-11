#pragma once
#include "light.h"

class OmnidirectionalLight : public Light
{
public:
	OmnidirectionalLight();
	~OmnidirectionalLight();
	// Getter
	float getCutoff() const;

private:
	float cutoff = 180.0f;
};
