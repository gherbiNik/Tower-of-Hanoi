
#include "engine.h"
#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "material.h"
#include "omnidirectionalLight.h"
#include "ovoReader.h"
#include <iostream>
#include <vector>
#include <iomanip>

 // --- GLOBALI ---
const float MAP_LIMIT = 55.0f;

Eng::Base* engine;
Camera* camera;
List* list;
Node* root;
OvoReader ovoreader{};
Node* tavoloNode;

bool isWon = false;
bool isRotationMode = false;

// Prototipi
void checkBoundaries();

// Helper client-side per centrare il testo usando API engine
void drawCenteredText(std::string text, float yOffset, float r, float g, float b) {
    int winW = engine->getWindowWidth();
    int winH = engine->getWindowHeight();
    int textWidth = engine->getTextWidth(text); // API Engine

    float x = (winW - textWidth) / 2.0f;
    float y = (winH / 2.0f) + yOffset;

    engine->addString(x, y, text, r, g, b); // API Engine
}

void displayCallback() {
    // 1. Logica Scena
    static float angle = 0.0f;
    angle += 0.5f;

    // 2. Setup Render List
    list->clear();
    list->pass(root, glm::mat4(1.0f));
    engine->setRenderList(list);
    engine->setMainCamera(camera);

    // 3. UI 
    engine->clearScreenText();

    // Debug Text (Alto Sinistra)
    if (isRotationMode)
        engine->addToScreenText("MODALITA': ROTAZIONE (WASD)");
    else
        engine->addToScreenText("MODALITA': MOVIMENTO (WASD)");

    engine->addToScreenText("[M] Cambia Modalita' | [ESC] Esci");
    if (!isWon) engine->addToScreenText("[V] Simula Vittoria");

    // Victory Screen (Centro Schermo)
    if (isWon) {
        drawCenteredText("HAI VINTO!", 0.0f, 0.2f, 1.0f, 0.2f); // Verde Lime
        drawCenteredText("Premi [R] per ricominciare", -30.0f, 1.0f, 1.0f, 1.0f); // Bianco
    }

    engine->postRedisplay();
}

void keyboardCallback(unsigned char key, int x, int y) {
    float moveSpeed = 5.0f;
    float rotSpeed = 2.0f;

    switch (key) {
    case 'm': case 'M':
        isRotationMode = !isRotationMode;
        break;

        // Movimento
    case 'w':
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(1, 0, 0));
        else                camera->translate(glm::vec3(0, 0, -moveSpeed));
        break;
    case 's':
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(1, 0, 0));
        else                camera->translate(glm::vec3(0, 0, moveSpeed));
        break;
    case 'a':
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(0, 1, 0));
        else                camera->translate(glm::vec3(-moveSpeed, 0, 0));
        break;
    case 'd':
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(0, 1, 0));
        else                camera->translate(glm::vec3(moveSpeed, 0, 0));
        break;
    case 'q':
        if (isRotationMode) camera->rotate(rotSpeed, glm::vec3(0, 0, 1));
        else                camera->translate(glm::vec3(0, moveSpeed, 0));
        break;
    case 'e':
        if (isRotationMode) camera->rotate(-rotSpeed, glm::vec3(0, 0, 1));
        else                camera->translate(glm::vec3(0, -moveSpeed, 0));
        break;

        // Logica Gioco
    case 'v': case 'V':
        isWon = true;
        break;

    case 'r': case 'R':
        if (isWon) {
            // Ricarica asset o resetta variabili
            tavoloNode = ovoreader.readFile("tavolo.ovo", "texture/");
            isWon = false;
        }
        break;

    case 27: exit(0); break;
    }

    if (!isRotationMode) checkBoundaries();
    engine->postRedisplay();
}

void reshapeCallback(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;
    camera->setProjectionMatrix(glm::perspective(glm::radians(45.0f), aspect, 1.0f, 5000.0f));
}

int main(int argc, char* argv[]) {
    engine = &Eng::Base::getInstance();
    if (!engine->init(argc, argv)) return -1;

    engine->createWindow(800, 600, 100, 100, "Tower of Hanoi");
    engine->enableFPS();

    engine->setKeyboardCallback(keyboardCallback);
    engine->setDisplayCallback(displayCallback);
    engine->setReshapeCallback(reshapeCallback);

    camera = new Camera("MainCam");
    camera->translate(glm::vec3(0.0f, 25.0f, 50.0f));

    list = new List();

    // Luci e Mesh
    OmnidirectionalLight* light = new OmnidirectionalLight();
    light->setPosition(glm::vec4(0.0f, 200.0f, 0.0f, 1.0f));

    tavoloNode = ovoreader.readFile("tavolo.ovo", "texture/");

    if (tavoloNode) {
        root = tavoloNode;
        root->addChild(light);
        root->addChild(camera);
    }
    else {
        // Fallback
        std::cerr << "Errore caricamento OVO" << std::endl;
        root = new Node("Root");
        root->addChild(camera);
    }

    engine->update();
    engine->free();

    delete list;
    delete camera;
    return 0;
}

void checkBoundaries() {
    float y_lower_bound = 5.0f;
    glm::mat4 mat = camera->getM();
    glm::vec3 pos = glm::vec3(mat[3]);
    bool collision = false;

    if (pos.x > MAP_LIMIT) { pos.x = MAP_LIMIT; collision = true; }
    else if (pos.x < -MAP_LIMIT) { pos.x = -MAP_LIMIT; collision = true; }

    if (pos.y > MAP_LIMIT) { pos.y = MAP_LIMIT; collision = true; }
    else if (pos.y < y_lower_bound) { pos.y = y_lower_bound; collision = true; }

    if (pos.z > MAP_LIMIT) { pos.z = MAP_LIMIT; collision = true; }
    else if (pos.z < -MAP_LIMIT) { pos.z = -MAP_LIMIT; collision = true; }

    if (collision) {
        mat[3][0] = pos.x; mat[3][1] = pos.y; mat[3][2] = pos.z;
        camera->setM(mat);
    }
}