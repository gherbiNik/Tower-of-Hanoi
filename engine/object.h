#pragma once

#include <string>
#include <memory>
#include <atomic>
#include "libConfig.h"

/**
 * @brief Classe base astratta per tutti gli oggetti della scena.
 */
class ENG_API Object {
private:
  
   static std::atomic<unsigned int> nextId;
public:
   Object(const std::string& name);

   virtual void render() = 0;
   virtual std::string getName() const;
   virtual unsigned int getId() const;
   virtual ~Object() = default;

protected:
    unsigned int uid;
    std::string name;
};