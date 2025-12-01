/**
 * @file		engine.cpp
 * @brief	Graphics engine main file
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch] << change this to your group members
 */



//////////////
// #INCLUDE //
//////////////

   // Main include:
   #include "engine.h"
   
   // C/C++:
   #include <iostream>   
   #include <source_location>
   #include <GL/freeglut.h>

   #include <glm/gtc/type_ptr.hpp>





/////////////////////////
// RESERVED STRUCTURES //
/////////////////////////

/**
 * @brief Base class reserved structure (using PIMPL/Bridge design pattern https://en.wikipedia.org/wiki/Opaque_pointer).
 */
struct Eng::Base::Reserved
{
   // Flags:
   bool initFlag;
   

   /**
    * Constructor.
    */
   Reserved() : initFlag{ false } 
   {}
};



////////////////////////
// BODY OF CLASS Base //
////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor.
 */
ENG_API Eng::Base::Base() : reserved(std::make_unique<Eng::Base::Reserved>())
{  
#ifdef _DEBUG   
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Destructor.
 */
ENG_API Eng::Base::~Base()
{
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Gets a reference to the (unique) singleton instance.
 * @return reference to singleton instance
 */
Eng::Base ENG_API &Eng::Base::getInstance()
{
   static Base instance;
   return instance;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Init internal components. 
 * @return TF
 */
bool ENG_API Eng::Base::init()
{
   // Already initialized?
   if (reserved->initFlag)
   {
      std::cout << "ERROR: engine already initialized" << std::endl;
      return false;
   }

   // Here you can initialize most of the graphics engine's dependencies and default settings...
   
   // Done:
   std::cout << "[>] " << LIB_NAME << " initialized" << std::endl;
   reserved->initFlag = true;
   return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Free internal components.
 * @return TF
 */
bool ENG_API Eng::Base::free()
{
   // Not initialized?
   if (!reserved->initFlag)
   {
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   // Here you can properly dispose of any allocated resource (including third-party dependencies)...

   // Done:
   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved->initFlag = false;
   return true;
}

void Eng::Base::loadFromFile(const char* file_path, const char* texture_dir)
{
}

void ENG_API Eng::Base::render(Camera* camera, List* list)
{
   if (!camera || !list) return;

   // 1. Pulisci i buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // 2. Imposta la matrice di Proiezione
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(glm::value_ptr(camera->getProjectionMatrix()));

   // 3. Calcola la View Matrix (inversa della camera)
   glm::mat4 viewMatrix = camera->getInvCameraMatrix();

   // 4. Renderizza la lista passando la View Matrix
   // (Il riempimento della lista 'list->pass()' viene fatto dal client prima di chiamare render)
   list->render(viewMatrix);

  
}

void Eng::Base::setCamera(Camera* camera)
{
   // TODO
}



void Eng::Base::mainEventLoop() {
   glutMainLoop();
}


void Eng::displayCallBack() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   glutSwapBuffers();
}

void ENG_API Eng::Base::registerDisplayCallback(void (*function)(void)) {
   glutDisplayFunc(displayCallBack);
}

void ENG_API Eng::Base::registerReshapeCallback(void (*function)(int, int)) {
   glutReshapeFunc(reshapeCallBack);
}

void Eng::reshapeCallBack(int width, int height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 1.0f, 150.0f);
   glLoadMatrixf(glm::value_ptr(projection));
   glMatrixMode(GL_MODELVIEW);
}
