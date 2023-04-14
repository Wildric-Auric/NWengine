#pragma once

#include "Globals.h"
#include <vector>

#define GET_GUI_ID NWGui::AutoInc++


#ifndef NW_GAME_BUILD

#define GUI_SEP ImGui::Separator();
#define GUI_NEWLINE ImGui::NewLine();
#define GUI_SAMELINE ImGui::SameLine();


enum GuiDataType_
{
    GuiDataType_S8,       // signed char / char (with sensible compilers)
    GuiDataType_U8,       // unsigned char
    GuiDataType_S16,      // short
    GuiDataType_U16,      // unsigned short
    GuiDataType_S32,      // int
    GuiDataType_U32,      // unsigned int
    GuiDataType_S64,      // long long / __int64
    GuiDataType_U64,      // unsigned long long / unsigned __int64
    GuiDataType_Float,    // float
    GuiDataType_Double,   // double
    GuiDataType_COUNT
};

class NWGui {
public:
	static uint32 AutoInc;
    static double* GetPlotArray();
    static void Text(std::string text);
	static bool FileHolder(std::string name, std::string holdedFile);
	static bool CheckBox(std::string label, bool* value);
	static bool Button(std::string label);
	static bool DragValue(const char* label, void* target, int dataType, int16 vectorSize = 1, float speed = 1.0f, float minn = 0.0f, float maxx = 0.0f);
	static std::string GenLabel(const char* string, uint32 id);
    static bool Input(std::string label, std::string* buffer);
    

	static bool QuadraticBezierPlot(std::string label, fVec2* source, fVec2* target, fVec2* controlPoint);
	static bool CubicBezierPlot(std::string label, fVec2* source, fVec2* target, fVec2* controlPoint, fVec2* controlPoint0);

	static bool QuadraticBezierInterpolationPlot(std::string label, float* source, float* target, fVec2* controlPoint);
	static bool CubicBezierInterpolationPlot(std::string label, float* source, float* target, fVec2* controlPoint, fVec2* controlPoint0);
};

#endif