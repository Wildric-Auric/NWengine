#pragma once

#ifdef _MSC_VER
#define DllExport extern "C" __declspec(dllexport)
#else
#define DllExport extern "C" __attribute__((visibility("default")))
#endif
#define DECL(retType, name, ...)                                                                                                 \
	typedef retType (*name##Proc)(__VA_ARGS__);                                                                                  \
	DllExport retType name(__VA_ARGS__);
