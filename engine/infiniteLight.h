#pragma once

#include "light.h"
#include "libConfig.h"

class ENG_API InfiniteLight : public Light
{
public:
	InfiniteLight();
	~InfiniteLight();
	void setDirection(const glm::vec3& dir);
	glm::vec3 getDirection() const;
	void render() override;
private:

};

