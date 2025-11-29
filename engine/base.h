#pragma once

#include <string>
#include "camera.h"
#include "list.h"


/**
 * @brief Implementato come Singleton.
 */
class Base {
private:
    Base();
    ~Base();

    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;

public:
    static Base& getInstance();

    void init();
    void free();
    };