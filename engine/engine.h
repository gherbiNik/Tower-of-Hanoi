/**
 * @file		engine.h
 * @brief	Graphics engine main include file
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch] << change this to your group members
 */
#pragma once


 
//////////////
// #INCLUDE //
//////////////

   // C/C++:         
   #include <memory>
#include <string>
#include "camera.h"
#include "list.h"
#include "light.h"
#include "libConfig.h"



/////////////
// VERSION //
/////////////

   // Generic info:
#ifdef _DEBUG
   #define LIB_NAME      "My Graphics Engine v0.1a (debug)"   ///< Library credits
#else
   #define LIB_NAME      "My Graphics Engine v0.1a"   ///< Library credits
#endif
   #define LIB_VERSION   10                           ///< Library version (divide by 10)





///////////////
// NAMESPACE //
///////////////

namespace Eng {



//////////////
// #INCLUDE //
//////////////   

   // You can subinclude here other headers of your engine...



///////////////////////
// MAIN ENGINE CLASS //
///////////////////////

/**
 * @brief Base engine main class. This class is a singleton.
 */
class ENG_API Base final
{
//////////
public: //
//////////	      

   // Const/dest:
   Base(Base const &) = delete;
   ~Base();

   // Operators:
   void operator=(Base const &) = delete;

   // Singleton:
   static Base &getInstance();

   // Init/free:
   bool init();
   bool free();   
   void loadFromFile(const char* file_path, const char* texture_dir);
   
   void mainEventLoop();
   friend void reshapeCallBack(int width, int height);
   friend void displayCallBack();
   void render(Camera* camera, List* list);

   /**
   * \brief Sets the engine's active camera.
   * \param camera Pointer to the PerspectiveCamera object.
   */
   void setCamera(Camera* camera);

   /**
 * \brief Registers a display callback.
 * \param function Pointer to the display callback function.
 */
    void registerDisplayCallback(void (*function)(void));

   /**
    * \brief Registers a reshape callback.
    * \param function Pointer to the reshape callback function.
    */
    void registerReshapeCallback(void (*function)(int, int));



///////////
private: //
///////////	

   // Reserved:
   struct Reserved;
   std::unique_ptr<Reserved> reserved;

   Camera* camera;
   /**
   * \brief Pointer to the user-defined reshape callback function.
   *
   * This function is invoked when the window is resized.
   * It takes the new width and height as arguments.
   */
   void (*reshape_function)(int, int) { nullptr };

   /**
    * \brief Pointer to the user-defined display callback function.
    *
    * This function is called to render the window content.
   */
   void (*display_function)(void) { nullptr };


   

   // Const/dest:
   Base();
};

}; // end of namespace Eng::

