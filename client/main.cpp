/**
 * @file		main.cpp
 * @brief	Client application (that uses the graphics engine)
 *
 * @authors	 Andrea Perlini, Nicholas Gherbi, Andrea Domenico Trapani 
 */



//////////////
// #INCLUDE //
//////////////

   // Library header:
   #include "engine.h"

   // C/C++:
   #include <iostream>



//////////
// MAIN //
//////////

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char *argv[])
{
   // Init engine:
   Eng::Base &eng = Eng::Base::getInstance();
   eng.init();


   // Loop principale
   //while (true) {
   eng.update();
   //}
   // Release engine:
   eng.free();

   // Done:
   std::cout << "\n[application terminated]" << std::endl;
   return 0;
}
