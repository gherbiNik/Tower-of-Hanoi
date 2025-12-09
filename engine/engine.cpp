#include "engine.h"
#include <iostream>   
#include <GL/freeglut.h>
#include "FreeImage.h"
#include <glm/gtc/type_ptr.hpp>

// --- PIMPL STRUCTURE ---
struct Eng::Base::Reserved
{
   bool initFlag = false;

   // --- DATI SCENA  ---
   Camera* currentCamera = nullptr;  
   List* currentList = nullptr;      

   // -- WINDOW STATE -- 
   int windowWidth = 800;  // Default
   int windowHeight = 600; // Default

   // -- FPS -- 
   bool show_fps = false;
   float fps = 0.0f;
   int frameCounter = 0;
   std::chrono::time_point<std::chrono::steady_clock> lastTime = std::chrono::steady_clock::now();

   // Queue of messages (Text to Render)
   std::vector<std::string> textToRender;

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
   FreeImage_Initialise();
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
/*
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
*/

void Eng::Base::render() {
   // Safety Check sui dati interni
   if (!reserved->currentCamera || !reserved->currentList) return;

   // ------------------------------------------
   //           RENDERING 3D (SCENE)
   // ------------------------------------------

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);

   glEnable(GL_TEXTURE_2D);

   // Se il rendering del testo (2D) ha cambiato il colore corrente (es. a nero),
   // e i materiali usano GL_COLOR_MATERIAL, gli oggetti 3D diventano neri.
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   
   // Clean buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Setup Proiezione
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(glm::value_ptr(reserved->currentCamera->getProjectionMatrix()));

   // Setup View Matrix
   glm::mat4 viewMatrix = reserved->currentCamera->getInvCameraMatrix();

   // Render List
   reserved->currentList->render(viewMatrix);


   // ------------------------------------------
   //            RENDERING 2D 
   // ------------------------------------------
   calculateFPS();

   // Disabilita effetti 3D non voluti sulla GUI
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST); // Disabilita Z-Buffer per scrivere SOPRA tutto

   // Imposta Proiezione Ortogonale (Sovrascrive quella 3D)
   glMatrixMode(GL_PROJECTION);
   glm::mat4 ortho = glm::ortho(0.0f, (float)reserved->windowWidth, 0.0f, (float)reserved->windowHeight, -1.0f, 1.0f);
   glLoadMatrixf(glm::value_ptr(ortho));

   // Resetta ModelView (Sovrascrive quella della camera)
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

   // Text color set to white
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   // IMPOSTAZIONI DISTANZA (Modifica questo valore per allontanare tutto dai bordi)
   float margin = 20.0f;
   float yStart = (float)reserved->windowHeight - margin;

   // --- 1. DISEGNO FPS (ALLINEATO A DESTRA) ---
   if (reserved->show_fps) {
      char fpsBuffer[64];
      // Creiamo la stringa completa
      snprintf(fpsBuffer, sizeof(fpsBuffer), "FPS: %.2f", reserved->fps);

      // Calcoliamo quanto è lunga in pixel questa specifica stringa
      int textWidth = glutBitmapLength(GLUT_BITMAP_8_BY_13, (unsigned char*)fpsBuffer);

      // Calcoliamo la X in modo che finisca esattamente a 'margin' dal bordo destro
      float fpsX = (float)reserved->windowWidth - textWidth - margin;

      glRasterPos2f(fpsX, yStart);
      glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)fpsBuffer);
   }

   // --- 2. DISEGNO TESTO (ALLINEATO A SINISTRA) ---
   float textYPosition = yStart; // Partiamo dalla stessa altezza degli FPS

   if (!reserved->textToRender.empty())
   {
      char buffer[256];
      for (const auto& toPrint : reserved->textToRender) {
         snprintf(buffer, sizeof(buffer), "%s", toPrint.c_str());

         // X fissa al margine sinistro
         glRasterPos2f(margin, textYPosition);

         glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char*)buffer);

         // Scendiamo per la riga successiva
         textYPosition -= 15.0f; // Spazio tra le righe
      }
   }
   // End 2D

   glEnable(GL_LIGHTING);
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

   // Updates internal data
   reserved->windowWidth = width;
   reserved->windowHeight = height;

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

void ENG_API Eng::Base::calculateFPS() {
    reserved->frameCounter++;
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - reserved->lastTime;

    if (elapsedTime.count() >= 1.0f) { // Updates every second
        reserved->fps = reserved->frameCounter / elapsedTime.count();
        reserved->frameCounter = 0;
        reserved->lastTime = currentTime;
    }
}

void ENG_API Eng::Base::enableFPS() {
    reserved->show_fps = true;
}

void ENG_API Eng::Base::disableFPS() {
    reserved->show_fps = false;
}

// Will be use by client to add text
void Eng::Base::addText(const std::string& text) {
    reserved->textToRender.push_back(text);
}

void Eng::Base::postRedisplay() {
   glutPostRedisplay();
}

void Eng::Base::addToScreenText(std::string text) {
    reserved->textToRender.push_back(text);
}

void Eng::Base::clearScreenText() {
    reserved->textToRender.clear();
}

void Eng::Base::drawText(const char* text, float x, float y) {
    glm::mat4 projectionMatrix;
    glm::mat4 modelViewMatrix;

    glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projectionMatrix));
    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelViewMatrix));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    // RESETTA MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // DISABILITA EFFETTI 3D
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // 5. DISEGNA IL TESTO
    glColor3f(0.2f, 1.0f, 0.2f); // Verde Lime
    glRasterPos2i((int)x, (int)y);

    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // RIPRISTINA FLAG
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // RIPRISTINA MATRICI
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projectionMatrix));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelViewMatrix));
}