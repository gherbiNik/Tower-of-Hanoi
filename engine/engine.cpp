#include "engine.h"
#include <iostream>   
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

// --- PIMPL STRUCTURE ---
struct Eng::Base::Reserved
{
   bool initFlag = false;

   // --- DATI SCENA  ---
   Camera* currentCamera = nullptr;  
   List* currentList = nullptr;      

   // Callback del Client
   Eng::DisplayCallback   clientDisplayCb = nullptr;
   Eng::ReshapeCallback  clientReshapeCb = nullptr;
   Eng::KeyboardCallback clientKeyboardCb = nullptr;
   Eng::SpecialCallback  clientSpecialCb = nullptr;
};

// --- STATIC WRAPPERS (Ponte C -> C++) ---
static void glutDisplayWrapper() {
   // Delega tutto all'istanza singleton
   Eng::Base::getInstance().handleDisplayRequest();
}

static void glutReshapeWrapper(int width, int height) {
   Eng::Base::getInstance().handleReshapeRequest(width, height);
}

static void glutKeyboardWrapper(unsigned char key, int x, int y) {
   Eng::Base::getInstance().handleKeyboardRequest(key, x, y);
}

static void glutSpecialWrapper(int key, int x, int y) {
   Eng::Base::getInstance().handleSpecialRequest(key, x, y);
}

// --- IMPLEMENTAZIONE BASE ---

Eng::Base::Base() : reserved(std::make_unique<Eng::Base::Reserved>()) {}
Eng::Base::~Base() { free(); }

Eng::Base& Eng::Base::getInstance() {
   static Base instance;
   return instance;
}

bool Eng::Base::init(int argc, char* argv[]) {
   if (reserved->initFlag) return false;

   // Inizializza GLUT internamente
   glutInit(&argc, argv);

   std::cout << "[Engine] Initialized" << std::endl;
   reserved->initFlag = true;
   return true;
}

bool Eng::Base::free() {
   if (!reserved->initFlag) return false;
   reserved->initFlag = false;
   std::cout << "[Engine] Freed" << std::endl;
   return true;
}

void Eng::Base::createWindow(int width, int height, int x, int y, const char* title) {
   if (!reserved->initFlag) {
      std::cerr << "Error: Engine not initialized. Call init() first." << std::endl;
      return;
   }

   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize(width, height);
   glutInitWindowPosition(x, y);
   glutCreateWindow(title);

   // Registra i wrapper statici
   glutDisplayFunc(glutDisplayWrapper);
   glutReshapeFunc(glutReshapeWrapper);
   glutKeyboardFunc(glutKeyboardWrapper);
   glutSpecialFunc(glutSpecialWrapper);

   // Impostazioni OpenGL di base
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glEnable(GL_LIGHTING);
   glEnable(GL_NORMALIZE); // Utile se scaliamo le mesh
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Eng::Base::update() {
   glutMainLoop();
}

// Setters per le callback
void Eng::Base::setDisplayCallback(DisplayCallback cb) { reserved->clientDisplayCb = cb; }
void Eng::Base::setReshapeCallback(ReshapeCallback cb) { reserved->clientReshapeCb = cb; }
void Eng::Base::setKeyboardCallback(KeyboardCallback cb) { reserved->clientKeyboardCb = cb; }
void Eng::Base::setSpecialCallback(SpecialCallback cb) { reserved->clientSpecialCb = cb; }

// Utility
void Eng::Base::setClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
void Eng::Base::setLighting(bool enable) { if (enable) glEnable(GL_LIGHTING); else glDisable(GL_LIGHTING); }
void Eng::Base::setDepthTest(bool enable) { if (enable) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST); }

void Eng::Base::setRenderList(List* list) {
   reserved->currentList = list; // Salviamo il puntatore per il rendering
}

void Eng::Base::setMainCamera(Camera* camera) {
   reserved->currentCamera = camera; // Salviamo il puntatore per il rendering
}

// Main Render Function
void Eng::Base::render(Camera* camera, List* list) {
   if (!camera || !list) return;

   // 1. Pulisci buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // 2. Setup Proiezione
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(glm::value_ptr(camera->getProjectionMatrix()));

   // 3. Ottieni View Matrix (Inversa della camera)
   glm::mat4 viewMatrix = camera->getInvCameraMatrix();

   // 4. Renderizza la lista
   list->render(viewMatrix);

   // 5. Swap Buffers
   glutSwapBuffers();
}

void Eng::Base::render() {
   // Safety Check sui dati interni
   if (!reserved->currentCamera || !reserved->currentList) return;

   // 1. Pulisci buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // 2. Setup Proiezione
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(glm::value_ptr(reserved->currentCamera->getProjectionMatrix()));

   // 3. Setup View Matrix
   glm::mat4 viewMatrix = reserved->currentCamera->getInvCameraMatrix();

   // 4. Render List
   reserved->currentList->render(viewMatrix);

   // 5. Swap
   glutSwapBuffers();
}

void Eng::Base::handleDisplayRequest() {
   // 1. Esegui la logica del client (animazioni, calcoli, riempimento lista)
   if (reserved->clientDisplayCb) {
      reserved->clientDisplayCb();
   }

   // 2. Esegui il rendering tecnico dell'engine
   // Nota: render() usa currentCamera e currentList che il client dovrebbe aver settato
   // dentro la clientRenderCb appena chiamata.
   this->render();
}

void Eng::Base::handleReshapeRequest(int width, int height) {
   if (height == 0) height = 1;

   // Aggiorna viewport OpenGL
   glViewport(0, 0, width, height);

   // Notifica il client (per ricalcolare la projection matrix)
   if (reserved->clientReshapeCb) {
      reserved->clientReshapeCb(width, height);
   }
}

void Eng::Base::handleKeyboardRequest(unsigned char key, int x, int y) {
   if (reserved->clientKeyboardCb) {
      reserved->clientKeyboardCb(key, x, y);
   }
}

void Eng::Base::handleSpecialRequest(int key, int x, int y) {
   if (reserved->clientSpecialCb) {
      reserved->clientSpecialCb(key, x, y);
   }
}