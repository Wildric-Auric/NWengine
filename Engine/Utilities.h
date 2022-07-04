#pragma once
#include "GameObject.h"
#include <string>
#include <vector>
#include <windows.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif


#define WIN 


inline bool IsColliding(Collider* collider1, Collider* collider2, Vector2<int> offset1 = Vector2<int>(0,0)) {
	uint8_t overlap = 0;
	if (collider1 == nullptr || collider2 == nullptr) return false;
	int minX1 = collider1->GetPosition().x + offset1.x -  collider1->GetSize().x / 2;
	int maxX1 = collider1->GetPosition().x + offset1.x +  collider1->GetSize().x/2;
	int minY1 = collider1->GetPosition().y + offset1.y -  collider1->GetSize().y/2;
	int maxY1 = collider1->GetPosition().y + offset1.y +  collider1->GetSize().y/2;

	int minX2 = collider2->GetPosition().x - collider2->GetSize().x / 2;
	int maxX2 = collider2->GetPosition().x + collider2->GetSize().x / 2;
	int minY2 = collider2->GetPosition().y - collider2->GetSize().y / 2;
	int maxY2 = collider2->GetPosition().y + collider2->GetSize().y / 2;

	overlap += (  
					( (minX1 <= minX2) && (minX2 <= maxX1) ) ||  
					( (minX2 <= minX1) && (minX1 <= maxX2) ) 
			   )
			+ 
			   ( 
					 ( (minY1 <= minY2) && (minY2 <= maxY1) ) ||
					 ( (minY2 <= minY1) && (minY1 <= maxY2) )
			   )
		;

	return overlap == 2;
}


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

inline std::vector<std::string> GetRecusivelyDirFiles(const std::string& directory){
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path = directory + "\\*";
	std::vector<std::string> dirList;

	hFind = FindFirstFileA(path.c_str(), &findData);
	bool first = 0;
	while (FindNextFileA(hFind, &findData) != 0)
	{
		if (first) {
			std::string fileName = std::string(findData.cFileName);
			dirList.push_back(fileName);
			if (fileName.find(".") == -1) {
				ExtendVector(&dirList, GetRecusivelyDirFiles(directory + "/" + fileName));
			}
		}
		first = 1;
	}

	FindClose(hFind);
	return dirList;

	if (hFind == INVALID_HANDLE_VALUE)
		throw std::runtime_error("Invalid handle value! Please check your path...");

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
		const char* r = "NWengine\\";
		std::string f = "";
		int counter = 0;
		bool add = 0;
		for (char c : filename) {
			if (c == r[counter]) counter++;
			else counter = 0;
			if (add) f += c;
			if (counter > 8) {
				add = 1;
			} 
		}
		if (add == 0) {
			std::cout << "ERROR::File not in NWengine directory" << std::endl; //Maybe try adding it to this directory LOG
			return "";
		}
		return f;
	}
	else
	{
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
	}
	return "";
}

inline std::string GetFileName(std::string path) {
	bool add = 0;
	std::string name = "";
	for (int i = path.size() - 1; i > -1; i--) {
		char c = path[i];
		if (c == '\\') break;
		if (add) name = c + name;
		if (c == '.') add = 1;

	};
	return name;
}

#endif