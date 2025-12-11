/**
 * @file		engine.h
 * @brief	Graphics engine main include file
 */
#pragma once

#include <memory>
#include <functional>
#include <string>
#include "libConfig.h"
#include "camera.h"
#include "list.h"
#include <chrono>


namespace Eng {

   // Definizioni dei tipi di callback per il Client
   using DisplayCallback = std::function<void()>;
   using ReshapeCallback = std::function<void(int width, int height)>;
   using KeyboardCallback = std::function<void(unsigned char key, int x, int y)>;
   using SpecialCallback = std::function<void(int key, int x, int y)>;

   class ENG_API Base final
   {
   public:
      // Singleton
      static Base& getInstance();

      // Ciclo di vita
      bool init(int argc, char* argv[]);
      bool free();

      void setRenderList(List* list);
      void setReflectionList(List* list);
      void setMainCamera(Camera* camera);

      // Gestione Finestra e Loop
      void createWindow(int width, int height, int x, int y, const char* title);
      void update(); // Avvia il main loop

      // Rendering
      //void render(Camera* camera, List* list);
      void render();

      // Callback Registration (Il client usa queste per agganciare la logica)
      void setDisplayCallback(DisplayCallback cb);
      void setReshapeCallback(ReshapeCallback cb);
      void setKeyboardCallback(KeyboardCallback cb);
      void setSpecialCallback(SpecialCallback cb);

      // Utility per il Client
      void setClearColor(float r, float g, float b, float a);
      void setLighting(bool enable);
      void setDepthTest(bool enable);
      void setTexture(bool enable);

      // --- METODI PER LA GESTIONE EVENTI (Chiamati dai wrapper statici) ---
      void handleDisplayRequest();
      void handleReshapeRequest(int width, int height);
      void handleKeyboardRequest(unsigned char key, int x, int y);
      void handleSpecialRequest(int key, int x, int y);

      // No copy
      Base(Base const&) = delete;
      void operator=(Base const&) = delete;

      void enableFPS();
      void disableFPS();
      void calculateFPS();
      void addText(const std::string& text);

      void addToScreenText(std::string text);
      void clearScreenText();
      void postRedisplay();
      void drawText(const char* text, float x, float y);

      void showVictory(const std::string& message);
      void hideVictory();

      void addString(float x, float y, std::string text, float r, float g, float b);
      int getWindowWidth();
      int getWindowHeight();
      int getTextWidth(const std::string& text);

   private:
      Base();
      ~Base();

      // PIMPL: Nasconde i dettagli privati (es. header GLUT)
      struct Reserved;
      std::unique_ptr<Reserved> reserved;
    
      ////FPS:
      bool show_fps;
      int frameCounter = 0;
      float fps = 0.0f;
      /*
      * Timestamp of the last frame update.
        Used to calculate the elapsed time between frames for FPS computation.
      */
      std::chrono::time_point<std::chrono::steady_clock> lastTime;
   
   };

} // namespace Eng