#include "object.h"

ENG_API Object::Object(unsigned int id, const std::string& name)
   : uid(id), name(name)
{
}

std::string Object::getName() const {
   return this->name;
}

unsigned int Object::getId() const {
   return this->uid;
}