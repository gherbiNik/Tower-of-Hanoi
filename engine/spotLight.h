#pragma once

#include "light.h"
#include "libConfig.h"

class ENG_API SpotLight : public Light
{
public:
	SpotLight();
	~SpotLight();

	void render() override;
	// Getter
	float getCutoff() const;
	// Setter
	void setCutoff(float value);

private:
	float cutoff;
};
