#pragma once

#include "Maths.h"  
#include <string>


//For now using just printf, should output to console later
#define NW_LOG_ERROR(str)	printf(str)
#define NW_LOG_WARNING(str) printf(str)
#define NW_LOG(str)			printf(str)

//#define NW_DLL_ENGINE       //comment this line if you want either dll or independent exe as output
#define NW_KEEP_DLL_RUNNING  1
#define NW_RELOAD_DLL        2
#define NW_SHUTDOWN_DLL      0
#define PROJECT_DIRECTORY    Globals::projectDir

//arch is always an integer type with size of pointer of the architecture
typedef long     arch;

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


typedef Vector2<int>	iVec2;
typedef Vector2<float>	fVec2;
typedef Vector3<float>	fVec3;
typedef Vector3<int>	iVec3;
typedef Vector2<double> dVec2;
typedef Vector3<double> dVec3;
typedef Vector4<int>	iVec4;
typedef Vector4<float>  fVec4;
typedef Vector4<double> dVec4;

typedef Matrix2<int>    iMat2;
typedef Matrix3<int>    iMat3;
typedef Matrix2<float>  fMat2;
typedef Matrix3<float>  fMat3;
typedef Matrix4<int>    iMat4;
typedef Matrix4<float>  fMat4;


enum DataTypes {
	NW_INT,
	NW_FLOAT,
	NW_DOUBLE,
	NW_VEC2,    //For shaders
	NW_VEC3,  
	NW_IVEC2,
	NW_IVEC3,
	NW_FVEC2,
	NW_FVEC3
};


enum EngineMode {
	PLAY_MODE,
	EDIT_MODE
};


class Globals {
public:
	//Consts
	static const float SCREENRATIO;
	//Paths
	static std::string scriptListPath;
	static std::string scriptManagerPath;
	static std::string installationDir;
	static std::string projDir;
	static std::string dllDir;
	static std::string compilationConfigDir;
	static std::string compiledScriptDir;
	static std::string engineLibDir;
	static std::string gamePath;
	static std::string gameLibDir;

	static void SetProjDir(std::string dir);
	static void SetInstallationDir(std::string dir);
};
