#pragma once

#include "light.h"
#include "libConfig.h"

class ENG_API SpotLight : public Light
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
