#pragma once

#include <string>
#include <memory>

/**
 * @brief Classe base astratta per tutti gli oggetti della scena.
 */
class Object {
public:
    Object(unsigned int id, const std::string& name);

    virtual void render() = 0;

    virtual std::string getName() const { return name; }
    virtual unsigned int getId() const { return uid; }

    virtual ~Object() = default;

private:
    unsigned int uid;
    std::string name;
};