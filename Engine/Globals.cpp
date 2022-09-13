#include "Globals.h"
#include <windows.h>

double Globals::fps = 60.0;
double Globals::deltaTime = 0.016;        //Read only in everyfile except mainloop
//Consts      
bool const Globals::DEBUG_MODE = true;
//Uniforms
float Globals::uTime = 0;
