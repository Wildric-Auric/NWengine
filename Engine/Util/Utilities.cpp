#include "Utilities.h"
#include <fstream>
#include <windows.h>
#include <wchar.h>
#include <codecvt>
#include <locale>

std::vector<std::string> GetNWlist(std::string path) {
	std::fstream stream(path); //TODO::ERROR checking
	std::vector<std::string> vec;
	for (std::string line = ""; std::getline(stream, line); vec.push_back(line));
	return vec;
}

void DllHandle::Load(const char* filename) {
	int a = sizeof(HMODULE);
	h = LoadLibrary(filename);
	if (!h || h == INVALID_HANDLE_VALUE) {
		h = nullptr;
		printf("Cannot load DLL\n");
	}
}

void* DllHandle::Get() {
    return h;
}

void DllHandle::Free() {
    if (h) 
        FreeLibrary((HINSTANCE)h);
}

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
std::vector<std::string> GetDirFiles(const std::string& directory, const std::string& extensionFilter)
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
		if (!first) {
			first = 1;
			continue;
		}
		std::string filename  = findData.cFileName;
		if (extensionFilter != "") {
			std::string extension = "";
			GetFileName(filename, nullptr, &extension);
			if (extension == extensionFilter)
				dirList.push_back(filename);
			continue;
		}
		dirList.push_back(filename);
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
	return std::string(dir) + "\\";
}

std::string GetExePath() {
	char path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	return std::string(path);
}

std::string GetFile(const char* type) {
	char filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; 
	ofn.lpstrFilter = type;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn)) {
		return std::string(filename);
	}
	return "";
}

std::string SaveAs(const char* type) {
	char filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; 
	ofn.lpstrFilter  = type;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Save as";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn)) {
		return std::string(filename);
	}
	return "";
}

std::string ToSingleBackSlash(const std::string& dir) {
	std::string ret = "";
	bool v = 0;
	for (char c : dir) {
		if (v && c == '\\')
			continue;
		if (c == '\\') {
			v = 1;
			ret += c;
			continue;
		}
		ret += c;
		v = 0;
	}
	return ret;
}

std::string ToDoubleBackSlash(const std::string& dir) {
	std::string ret = "";
	bool v = 0;
	for (char c : dir) {
		if (v && c == '\\')
			continue;
		if (c == '\\') {
			v = 1;
			ret += "\\\\";
			continue;
		}
		ret += c;
		v = 0;
	}
	return ret;
}


bool CopyDirectory(const std::string& dest, const std::string& src) {
	SHFILEOPSTRUCTW s   = { 0 };
	std::wstring tempSrc = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(src) + L"*";
	std::wstring tempDst = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(dest);


	WCHAR    src0[MAX_PATH + 1];
	WCHAR   dest0[MAX_PATH + 1];
	wcscpy_s(src0 , MAX_PATH, tempSrc.c_str());
	wcscpy_s(dest0, MAX_PATH, tempDst.c_str());
	 src0[lstrlenW(src0)+1]   = 0;
	dest0[lstrlenW(dest0)+1]  = 0;

	s.wFunc				= FO_COPY;
	s.fFlags			= FOF_SILENT;
	s.pTo			    = dest0;
	s.pFrom				= src0;

	int	res = SHFileOperationW(&s);
	return !res;
} 

bool MakeDir(const std::string& path) {
	return CreateDirectory(path.c_str(), NULL);
}

bool MakeFile(const std::string& path) {
	HANDLE h = CreateFileA(path.c_str(),FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	if (h == INVALID_HANDLE_VALUE)
		return 0;
	CloseHandle(h);
	return 1;
}

//Returns filename + extension
std::string GetFileName(const std::string& path, std::string* bFilename, std::string* bExtension, std::string* bRoot) {
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
			root += filename + "\\";
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


bool FileCopy(const std::string& dest, const std::string& src, bool failIfExists) {
	return CopyFile(src.c_str(), dest.c_str(), failIfExists);
}

bool FileExists(const std::string& dir) {
	DWORD attrib = GetFileAttributes(dir.c_str());
	if ((attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
		return 1;
	return 0;

}

bool Exec(const std::string& cmd, char* env) {
	STARTUPINFO			sInfo;
	PROCESS_INFORMATION pInfo;
	ZeroMemory(&sInfo, sizeof(sInfo));
	ZeroMemory(&pInfo, sizeof(pInfo));

	if (!CreateProcess(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, 0,
		env, NULL, &sInfo, &pInfo)) { 
		return 0;
	}

	WaitForSingleObject(pInfo.hProcess, INFINITE);
	DWORD exitCode;
	GetExitCodeProcess(pInfo.hProcess, &exitCode);
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
	return !exitCode;
}

bool GetVcVarsEnv(std::vector<char>& env) {
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;
    ZeroMemory(&sInfo, sizeof(sInfo));
    ZeroMemory(&pInfo, sizeof(pInfo));
    #ifdef _WIN64 
    char cmd[] = "cmd.exe /C \"vcvars64.bat > nul & set\""; 
    #else
    char cmd[] = "cmd.exe /C \"vcvars32.bat > nul & set\""; 
    #endif
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = 0;
    sa.bInheritHandle= TRUE;
    HANDLE rdPipe, wrtPipe;
    CreatePipe(&rdPipe, &wrtPipe, &sa, 0);
    SetHandleInformation(rdPipe, HANDLE_FLAG_INHERIT, 0);
    sInfo.dwFlags = STARTF_USESTDHANDLES;
    sInfo.hStdOutput = wrtPipe;
    sInfo.hStdError = wrtPipe;
    
    bool success = CreateProcess(NULL,cmd,NULL,NULL,TRUE,CREATE_NO_WINDOW, nullptr, 
                nullptr, &sInfo, &pInfo);
    if (!success) return 0;
    CloseHandle(wrtPipe);
    env.clear();
    int buffSize = 4096;
    env.resize(buffSize);
    DWORD bytes = 0;
    DWORD ptr = 0;
    int resizeFactor;
    while (ReadFile(rdPipe, env.data() + ptr, buffSize-1, &bytes, nullptr) && bytes > 0) {
        ptr   += bytes;
        resizeFactor = buffSize + ptr;
        if (resizeFactor <= env.size())
            continue;
        env.resize(buffSize + ptr);
    }
    env[ptr] = '\0';
    int i = 0;
    int j = 0;
    for (int j = 0; j < env.size(); ++j) {
        if (env[j] == '\r' && env[j+1] == '\n') {
            env[i] = '\0';
            j+=1;
        }
        else env[i] = env[j];
        i++;
    }
    WaitForSingleObject(pInfo.hProcess, INFINITE);
    CloseHandle(pInfo.hProcess);
    CloseHandle(pInfo.hThread);
    
    return 1;
}

bool FileDelete(const std::string& name) {
	return DeleteFile(name.c_str());
};

bool FileMove(const std::string& dest, const std::string& source, bool failIfExists) {
	if (!FileExists(source))
		return 0; 
	if (!failIfExists)
		FileDelete(dest);
	return MoveFile(source.c_str(), dest.c_str());
};
