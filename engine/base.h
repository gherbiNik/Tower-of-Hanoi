#pragma once

#include <string>

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
    void loadFromFile(const std::string& fileName);
    void swapBuffers();
    void mainEventLoop();
    void render();
};