#include "Globals.h"
#include <windows.h>

double Globals::fps = 60.0;
double Globals::deltaTime = 0.016;        //Read only in everyfile except mainloop
//Consts
int Globals::NATIVE_WIDTH = 850;			 
int Globals::NATIVE_HEIGHT = 640;          
const float Globals::SCREENRATIO = ((float)NATIVE_WIDTH) / ((float)NATIVE_HEIGHT);
int Globals::WINDOW_WIDTH	 = GetSystemMetrics(SM_CXSCREEN); //850;
int Globals::WINDOW_HEIGHT    = GetSystemMetrics(SM_CYSCREEN); //640
bool const Globals::DEBUG_MODE = true;
//Uniforms
float Globals::uTime = 0;
