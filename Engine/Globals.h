#pragma once
#include "Maths.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define NW_DLL_ENGINE       //comment this line if you want either dll or independent exe as output
#define NW_KEEP_DLL_RUNNING  1
#define NW_RELOAD_DLL        2
#define NW_SHUTDOWN_DLL      0


typedef int8_t   int8;
typedef int32_t  int32;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;


typedef uint8_t   uint8;
typedef uint32_t  uint32;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;


typedef Vector2<int> iVec2;
typedef Vector2<float> fVec2;
typedef Vector3<float> fVec3;
typedef Vector3<int> iVec3;
typedef Vector2<double> dVec2;
typedef Vector3<double> dVec3;

typedef Matrix<2, 2, int> iMat2;
typedef Matrix<3, 3, int>  iMat3;
typedef Matrix<2, 2, float> fMat2;
typedef Matrix<3, 3, float> fMat3;


class Globals {
public:
	static double fps;
	static double deltaTime;      //Read only in everyfile except mainloops
	//Consts
	static int NATIVE_WIDTH;
	static int NATIVE_HEIGHT;
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;
	static const float SCREENRATIO;
	static const bool DEBUG_MODE;
	//Uniforms
	static float uTime;

};
