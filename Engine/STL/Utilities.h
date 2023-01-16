#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <iostream>
#ifndef NOMINMAX
#define NOMINMAX
#endif


#define WIN 

template<typename T> 
void ExtendVector(std::vector<T>* a, std::vector<T> b) {
	for (auto c : b) {
		a->push_back(c);
	}
}


//Next functions use windows api; LATETODO:: Add their equivelent for mac, linux etc...
#ifdef WIN

/*See Interface.cpp to know these functions role. I could have used only one loop to get number and string but it's cleaner when done like
* like this and think it won't affect too much engine performance.
*/

struct DllHandle
{
	DllHandle(const char* filename) {
		h = LoadLibrary(filename);
		if (!h || h == INVALID_HANDLE_VALUE)
			printf("Cannot load DLL\n");
	}
	~DllHandle() {if (h) FreeLibrary(h);}
	const HINSTANCE Get() const { return h; }

private:
	HINSTANCE h;
};


inline void* GetDllFunction(DllHandle* dll, const char* functionName) {
	return GetProcAddress(dll->Get(), functionName);
}
inline std::vector<int> GetRecusivelyFilesNumber(const std::string& directory) {
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path = directory + "\\*";
	std::vector<int> dirList;

	hFind = FindFirstFileA(path.c_str(), &findData);
	int count = -1;
	while (FindNextFileA(hFind, &findData) != 0)
	{

		if (count >= 0) {
			std::string fileName = std::string(findData.cFileName);
			if (fileName.find(".") == -1) {
				ExtendVector(&dirList, GetRecusivelyFilesNumber(directory + "/" + fileName));
			}
			else dirList.push_back(0);
		}
		count += 1;
	}
	dirList.insert(dirList.begin(), count);

	FindClose(hFind);

	return dirList;
}
//DevNote: filesystem could be used here if C++ is minimum +17; since my intention is to use only
//C++ 11 I did not use it; instead I use windows api; which makes the application not cross plaform for now
inline std::vector<std::string> GetDirFiles(const std::string& directory)
{
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path = directory + "\\*";
	std::vector<std::string> dirList;

	hFind = FindFirstFileA(path.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Invalid handle value! Please check your path...");

	bool first = 0; //first file found is always ".." and I don't know why so i'm not adding it to the list
	while (FindNextFileA(hFind, &findData) != 0)
	{
		if  (first) dirList.push_back(std::string(findData.cFileName));
		first = 1;
	}

	FindClose(hFind);
	return dirList;
}

inline std::vector<std::string> GetRecusivelyDirFiles(const std::string& directory) {
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path = directory + "\\*";
	std::vector<std::string> dirList;

	hFind = FindFirstFileA(path.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE) {
		std::cout << "ERROR::Can't find path: " << path << std::endl;
		return dirList;
	}

	bool first = 0;
	while (FindNextFileA(hFind, &findData) != 0)
	{
		if (!first) {
			first = 1;
			continue;
		}

		std::string fileName = std::string(findData.cFileName);

		if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) //Is directory
			ExtendVector(&dirList, GetRecusivelyDirFiles(directory + "\\" + fileName));
		else
			dirList.push_back(directory + "\\" + fileName);
	}

	FindClose(hFind);
	return dirList;
}

inline int AccumulateChildren(std::vector<int>* a, std::vector<int>* b, int index = 0) {
	int count = 0;
	std::cout <<"a  "<< a->size() << "  " << index << '\n';
	int children = (*a)[index];
	count += children;
	int last = 1;
	for (int i = 0; i < children; i++) {
		int temp = AccumulateChildren(a, b, index + last);
		last += temp + 1;
		count += temp;
	}

	(*b)[index] = count;

	return count;
}

inline std::string GetCurrentDir() {
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	return std::string(dir);
}

inline std::string GetExePath() {
	char path[MAX_PATH];
	GetModuleFileName(NULL,path, MAX_PATH);
	return std::string(path);
}

inline std::string GetFile(const char* type = "Text Files\0*.txt\0*.*\0") {
	//Thank you Duthomhas
	char filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = type;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn)) {
		return std::string(filename);
	}

	// All this stuff below is to tell you exactly how you messed up above. 
	// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
	switch (CommDlgExtendedError())
	{
	case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
	case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
	case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
	case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
	case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
	case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
	case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
	case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
	case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
	case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
	case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
	case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
	case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
	case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
	case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
	default: return "";
	}
	return "";
}

//Returns filename + extension
inline std::string GetFileName(std::string path, std::string* bFilename = nullptr, std::string* bExtension = nullptr, std::string* bRoot = nullptr) {
	std::string filename  = "";
	std::string extension = "";
	std::string root	  = "";
	bool state = 0;
	for (auto chr : path) {
		if (chr == '.') {
			filename += extension;
			extension = "";
			state = 1;
		}
		if (chr == '\\') {
			root += filename + "\\";
			filename = "";
			extension = "";
			continue;
		}
		if (!state) filename += chr;
		else extension += chr;
	}
	if (bFilename  != nullptr) *bFilename  = filename;
	if (bExtension != nullptr) *bExtension = extension;
	if (bRoot      != nullptr) *bRoot      = root;
	return filename + extension;
}


inline bool FileCopy(std::string dest, std::string src, bool failIfExists = 0) {
	return CopyFile(src.c_str(), dest.c_str(), failIfExists);
}

inline int Exec(std::string cmd ) {
	STARTUPINFO			sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	if (!CreateProcess(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, 0,
					   NULL, NULL, &sInfo, &pInfo))
		return 0;

	WaitForSingleObject(pInfo.hProcess, INFINITE);
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
	return 1;
}
inline bool CopyDir() {}

#endif