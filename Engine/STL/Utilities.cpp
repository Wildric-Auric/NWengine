#include "Utilities.h"
#include <fstream>
#include <windows.h>

std::vector<std::string> GetNWlist(std::string path) {
	std::fstream stream(path); //TODO::ERROR checking
	std::vector<std::string> vec;
	for (std::string line = ""; std::getline(stream, line); vec.push_back(line));
	return vec;
}


DllHandle::DllHandle(const char* filename) {
	HINSTANCE temp = LoadLibrary(filename);
	if (!h || h == INVALID_HANDLE_VALUE)
		printf("Cannot load DLL\n");
	h = temp;
}

DllHandle::~DllHandle() { if (h) FreeLibrary((HINSTANCE)h); }

const void* DllHandle::Get() const { return h; }




void* GetDllFunction(DllHandle* dll, const char* functionName) {
	return GetProcAddress((HINSTANCE)dll->Get(), functionName);
}

std::vector<int> GetRecusivelyFilesNumber(const std::string& directory) {
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
std::vector<std::string> GetDirFiles(const std::string& directory)
{
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path = directory + "\\*";
	std::vector<std::string> dirList;

	hFind = FindFirstFileA(path.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
		return {};

	bool first = 0;
	while (FindNextFileA(hFind, &findData) != 0)
	{
		if (first) dirList.push_back(std::string(findData.cFileName));
		first = 1;
	}

	FindClose(hFind);
	return dirList;
}

std::vector<std::string> GetRecusivelyDirFiles(const std::string& directory) {
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path = directory + "\\*";
	std::vector<std::string> dirList;

	hFind = FindFirstFileA(path.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE) {
		printf("ERROR::Can't find path: %s\n", path.c_str());
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

inline int AccumulateChildren(std::vector<int>* a, std::vector<int>* b, int index) {
	int count = 0;
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

std::string GetCurrentDir() {
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	return std::string(dir);
}

std::string GetExePath() {
	char path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	return std::string(path);
}

std::string GetFile(const char* type) {
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
	case CDERR_DIALOGFAILURE:  printf("CDERR_DIALOGFAILURE\n");		break;
	case CDERR_FINDRESFAILURE: printf("CDERR_FINDRESFAILURE\n");	break;
	case CDERR_INITIALIZATION: printf("CDERR_INITIALIZATION\n");	break;
	case CDERR_LOADRESFAILURE: printf("CDERR_LOADRESFAILURE\n");	break;
	case CDERR_LOADSTRFAILURE: printf("CDERR_LOADSTRFAILURE\n");	break;
	case CDERR_LOCKRESFAILURE: printf("CDERR_LOCKRESFAILURE\n");	break;
	case CDERR_MEMALLOCFAILURE: printf("CDERR_MEMALLOCFAILURE\n");	break;
	case CDERR_MEMLOCKFAILURE:  printf("CDERR_MEMLOCKFAILURE\n");	break;
	case CDERR_NOHINSTANCE:		printf("CDERR_NOHINSTANCE\n");		break;
	case CDERR_NOHOOK:			printf("CDERR_NOHOOK\n");			break;
	case CDERR_NOTEMPLATE:		printf("CDERR_NOTEMPLATE\n");		break;
	case CDERR_STRUCTSIZE:		printf("CDERR_STRUCTSIZE\n");		break;
	case FNERR_BUFFERTOOSMALL:	printf("FNERR_BUFFERTOOSMALL\n");	break;
	case FNERR_INVALIDFILENAME: printf("FNERR_INVALIDFILENAME\n");	break;
	case FNERR_SUBCLASSFAILURE: printf("FNERR_SUBCLASSFAILURE\n");	break;
	default: return "";
	}
	return "";
}

//Returns filename + extension
std::string GetFileName(std::string path, std::string* bFilename, std::string* bExtension, std::string* bRoot) {
	std::string filename = "";
	std::string extension = "";
	std::string root = "";
	bool state = 0;
	bool slash = 0;
	for (auto chr : path) {
		if (chr == '.') {
			filename += extension;
			extension = "";
			state = 1;
			slash = 0;
		}
		if (chr == '\\') {
			if (slash) continue;
			root += filename + "\\\\";
			filename = "";
			extension = "";
			slash = 1;
			continue;
		}
		slash = 0;
		if (!state) filename += chr;
		else extension += chr;
	}
	if (bFilename != nullptr) *bFilename = filename;
	if (bExtension != nullptr) *bExtension = extension;
	if (bRoot != nullptr) *bRoot = root;
	return filename + extension;
}


bool FileCopy(std::string dest, std::string src, bool failIfExists) {
	return CopyFile(src.c_str(), dest.c_str(), failIfExists);
}

bool FileExists(std::string dir) {
	DWORD attrib = GetFileAttributes(dir.c_str());
	if ((attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
		return 1;
	return 0;

}

bool Exec(std::string cmd) {
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

bool FileDelete(std::string name) {
	return DeleteFile(name.c_str());
};

bool FileMove(std::string dest, std::string source, bool failIfExists) {
	if (!FileExists(source))
		return 0;
	if (!failIfExists)
		FileDelete(dest);
	return MoveFile(source.c_str(), dest.c_str());
};