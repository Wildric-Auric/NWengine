#include "HeaderAssembler.h"
#include "fstream"
#include <map>

#define WINDOWS_BLD
#ifdef  WINDOWS_BLD
#include <Windows.h>

void ToLower(std::string& str) {
    for (int i = 0; i < str.size();++i) {
        str[i] = std::tolower(str[i]);
    }
}

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
		std::string filename = findData.cFileName;
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
bool FileExists(std::string dir) {
	DWORD attrib = GetFileAttributes(dir.c_str());
	if ((attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
		return 1;
	return 0;

}
#endif


enum class State {
	Beg,
	Directive,
	Inlcude,
	IncludeBeg,
	Fail
};



std::map<std::string, bool> included;
std::ofstream outputF;

void Inc(const std::string& directory, const std::string& filename) {
	std::string path = directory + "\\" + filename;
	std::ifstream inp(path);
	if (!inp) {
		inp.close();
		printf("Cannot open file, check src directory");
	}
	std::string line;
	State s = State::Beg;
	while (std::getline(inp, line)) {
		std::string temp0 = "";
		s = State::Beg;
		for (size_t i = 0; i < line.size(); ++i) {
			char c = line[i];
			if (s == State::Beg) {
				if (c == '\t' || c == ' ') {
					outputF << c;
				}
				if (c == '#') {
					s = State::Directive;
				}
				else {
					outputF << c;
					s = State::Fail;
				}
			}
			else if (s == State::Directive) {
				const char* cmp = "include";
				std::string temp = "";
				State s0 = State::Inlcude;
				for (size_t j = 0; j < 7; j++) {
					if (i + j < line.size() && cmp[j] == line[i + j]) {
						temp += cmp[j];
						continue;
					}
					outputF << "#" << temp;
					if (i + j < line.size())
						i = max(i + j - 1, 0);
					s0 = State::Fail;
					break;
				}
				s = s0;
			}

			else if (s == State::Inlcude) {
				std::string name = "";
				if (c == '\t' || c == ' ') {
					continue;
				}
				if (c == '\"' || c == '<')
					s = State::IncludeBeg;
			}

			else if (s == State::IncludeBeg) {
				if (c == '\"' || c == '>') {
                    ToLower(temp0);
					if (FileExists(directory + "\\" + temp0)) {
						if (included.find(temp0) == included.end())
							Inc(directory, temp0);
						temp0 = "";
						continue;
					}
					if (included.find(temp0) == included.end())
						outputF << "#include<" << temp0 << ">";
					included.emplace(temp0, 1);
					temp0 = "";
				}
				temp0 += c;
			}
			else if (s == State::Fail) {
				outputF << c;
			}
		}
		outputF << "\n";
	}
	included.insert(std::make_pair(filename, 1));
	inp.close();
}


void AssembleHeaders(const std::string& src, const std::string& dst, const std::string& outputName, const std::string& first) {
	std::vector<std::string> vec = GetDirFiles(src);
    if (!first.empty()) {
        int index = -1;
	    for (int i = 0; i < vec.size(); ++i) {
            if (vec[i] != first) continue;
            index = i; 
            break;
        }
        if (index == -1) {
            printf("\"first\" file not found, it is ignored");
        }
        else {
            std::swap(vec[0], vec[index]);
        }
    }
	outputF.open(dst + "\\" + outputName);
	if (!outputF) {
		printf("Error openning dst file, check existence of output folder");
		outputF.close();
		return;
	}
	outputF << "#pragma once\n";
	for (std::string& file : vec) {
        ToLower(file);
		if (included.find(file) != included.end())
			continue;
		Inc(src, file);
	}
	outputF.close();
}
