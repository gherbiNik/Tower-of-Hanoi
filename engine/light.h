#pragma once
#include "node.h"
#include "libConfig.h"
#include <glm/glm.hpp>

class ENG_API Light : public Node
{
public:
    Light();
    Light(const std::string& name, const glm::mat4& matrix);
    virtual ~Light() = 0;

    const glm::vec4& getAmbient() const;
    const glm::vec4& getDiffuse() const;
    const glm::vec4& getSpecular() const;
    const glm::vec4& getPosition() const;

    void setAmbient(const glm::vec4& ambient);
    void setDiffuse(const glm::vec4& diffuse);
    void setSpecular(const glm::vec4& specular);
    void setPosition(const glm::vec4& position);

    void setLightID(int id);

    virtual void render() override;

protected:
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 position;
    int lightContextID;
};