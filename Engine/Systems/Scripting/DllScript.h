#pragma once

#define DllExport   extern "C" __declspec(dllexport)
#define DECL(retType, name, ...) typedef retType(*name##Proc)(__VA_ARGS__);DllExport retType name( __VA_ARGS__);

