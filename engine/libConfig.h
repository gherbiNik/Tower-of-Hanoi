#pragma once

// Export API:
#ifdef _WINDOWS
   // Specifies i/o linkage (VC++ spec):
#ifdef ENGINE_EXPORTS
#define ENG_API __declspec(dllexport)
#else
#define ENG_API __declspec(dllimport)
#endif      

// Get rid of annoying warnings:
#pragma warning(disable : 4251) 
#else // Under linux
#define ENG_API
#endif
