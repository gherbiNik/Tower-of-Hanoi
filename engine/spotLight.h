#pragma once

#include "light.h"

class SpotLight : public Light
{
public:
	SpotLight();
	~SpotLight();

	// Getter
	float getCutoff() const;
	// Setter
	void setCutoff(float value);

private:
	float cutoff;
};
