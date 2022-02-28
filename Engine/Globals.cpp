#include "Globals.h"
#include <windows.h>
double deltaTime = 0.016;        //Read only in everyfile except mainloops
iVec2 grav = iVec2(0.0, 10);
//Consts
int ORIGINAL_WIDTH = 850;			 //GetSystemMetrics(SM_CXSCREEN); //850
int ORIGINAL_HEIGHT = 640;           //GetSystemMetrics(SM_CYSCREEN); //640
const float SCREENRATIO = ((float)ORIGINAL_WIDTH) / ((float)ORIGINAL_HEIGHT);
int	RENDERING_WIDTH  = ORIGINAL_WIDTH;
int RENDERING_HEIGHT = ORIGINAL_HEIGHT;
int WINDOW_WIDTH	 = ORIGINAL_WIDTH;
int WINDOW_HEIGHT    = ORIGINAL_HEIGHT;
bool const DEBUG_MODE = true;
bool isMouseOnGui = false;

//Uniforms
float uTime = 0;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix(1.0);
