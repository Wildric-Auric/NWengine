#pragma once

/*


					 /$$   /$$ /$$      /$$                               /$$
					| $$$ | $$| $$  /$ | $$                              |__/
					| $$$$| $$| $$ /$$$| $$  /$$$$$$  /$$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$
					| $$ $$ $$| $$/$$ $$ $$ /$$__  $$| $$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
					| $$  $$$$| $$$$_  $$$$| $$$$$$$$| $$  \ $$| $$  \ $$| $$| $$  \ $$| $$$$$$$$
					| $$\  $$$| $$$/ \  $$$| $$_____/| $$  | $$| $$  | $$| $$| $$  | $$| $$_____/
					| $$ \  $$| $$/   \  $$|  $$$$$$$| $$  | $$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
					|__/  \__/|__/     \__/ \_______/|__/  |__/ \____  $$|__/|__/  |__/ \_______/
																/$$  \ $$
																   |  $$$$$$/
																\______/


														MIT License

						Copyright (c) 2022 Oussama Soudassi

						Permission is hereby granted, free of charge, to any person obtaining a copy
						of this software and associated documentation files (the "Software"), to deal
						in the Software without restriction, including without limitation the rights
						to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
						copies of the Software, and to permit persons to whom the Software is
						furnished to do so, subject to the following conditions:

						The above copyright notice and this permission notice shall be included in all
						copies or substantial portions of the Software.

						THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
						IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
						FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
						AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
						LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
						OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
						SOFTWARE.
*/

#define NW_NO_MANGLING extern "C"
#ifdef NW_DLL
#define NW_EXPORT __declspec(dllexport)
#else
#define NW_EXPORT
#endif
#define NW_PREFIX NW_NO_MANGLING NW_EXPORT

enum class ON_MAIN_CALL_LOCATION {
	InitBegin,		   // Executed just after context initialization
	InitEnd,		   // Executed at the end of engine init, after initialization of engine system
	FrameBegin,		   // Executed at beginning of each frame
	FrameIntermediate, // Executed each frame just before updating context buffer
	FrameEnd,		   // Executed each frame after updating context
	Destroy			   // Called before destruction of engine systems
};

struct NWengineVersion {
	int major;
	int minor;
	int day;
	int month;
	int year;
};

NW_PREFIX const char* NWengineGetVersionString();
NW_PREFIX void		  NWengineGetVersion(NWengineVersion* outVers);

extern const char* NWVersionString;
// extern std::unordered_map<ON_MAIN_CALL_LOCATION, std::vector<void (*)()>> functionMap;

NW_PREFIX int  NWengineInit();
NW_PREFIX void NWengineStep();
NW_PREFIX void NWengineLoop();
NW_PREFIX void NWengineShutdown();
NW_PREFIX bool NWenginePushFunction(ON_MAIN_CALL_LOCATION loc, void (*func)());
NW_PREFIX bool NWenginePopFunction(ON_MAIN_CALL_LOCATION loc, void (*func)() = nullptr);
