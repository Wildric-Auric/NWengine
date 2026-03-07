#include "HeaderAssembler.h"
#include <fstream>
#include <string.h>
#include <map>
#define maxx(a,b) ((a)>(b)?(a):(b))
#define minn(a,b) ((a)<(b)?(a):(b))

struct NWFile {
    void* handle;
    bool isDir = 0;
    char name[256];
};

#ifdef _WIN32 
#include <windows.h>
bool GetNextFile(NWFile* file) {
    WIN32_FIND_DATAA fd; 
    bool ret;
    if (!file->handle || file->handle == INVALID_HANDLE_VALUE) return 0; 
    ret = FindNextFileA(file->handle, &fd);
    if (!ret) {
        FindClose(file->handle);
        file->handle = INVALID_HANDLE_VALUE;
        return 0;
    }
    file->isDir  = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY; 
    memcpy(file->name, fd.cFileName, 256);
    return ret;
}

bool GetFirstFile(NWFile* file, const char* path) {
    WIN32_FIND_DATAA fd; 
	std::string	path2 = path + std::string("\\*");
    file->handle = INVALID_HANDLE_VALUE;
    file->handle = FindFirstFileA(path2.c_str(), &fd);
    file->isDir  = fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY; 
    return file->handle != INVALID_HANDLE_VALUE;
}

bool FileExists(const std::string& dir) {
	DWORD attrib = GetFileAttributes(dir.c_str());
	if((attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
		return 1;
	return 0;
}
#else

#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct dirent dir_entry;
typedef struct stat stat_t;

bool GetNextFile(NWFile* file) {
    stat_t st;
    dir_entry* ent;
    int res;
    DIR* fd = (DIR*)file->handle;
    if (!fd)
        return 0;
    ent = readdir(fd);
    if (!ent) {
        file->handle = 0;
        return 0;
    }
    res = stat(ent->d_name, &st);
    file->isDir = !res && st.st_mode & S_IFDIR;
    memcpy(file->name, ent->d_name, 256);
    return 1;
}

bool GetFirstFile(NWFile* file, const char* path) {
    DIR* fd;
    stat_t st;
    dir_entry* ent;
    int res;
    fd = opendir(path);
    file->handle = fd;
    return GetNextFile(file);
}

bool FileExists(const std::string& path) {
    return !access(path.c_str(), F_OK);
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

#ifdef _WIN32
#define sep "\\"
#else
#define sep "/"
#endif
void Inc(const std::string& directory, const std::string& filename) {
	std::string path = directory + sep + filename;
	std::ifstream inp(path);
	if (!inp) {
		inp.close();
		printf("Cannot open file, check src directory\n");
	}
	std::string line;
	State s = State::Beg;
    bool noBreak;
	while (std::getline(inp, line)) {
		std::string temp0 = "";
		s = State::Beg;
        noBreak = 0;
		for (size_t i = 0; i < line.size(); ++i) {
			char c  = line[i];
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
				static const char* cmp = "include";
                static const char* cmp2 = "pragma once";
				std::string temp = "";
				State s0 = State::Inlcude;
                size_t j;
				for (j = 0; j < 11; j++) {
					if (i + j < line.size() && cmp2[j] == line[i + j])
						continue;
					break;
				}
                if (j == 11) {
                    //we skip pragma once        
                    i = line.size();
                    j = 8;
                    noBreak = 1;
                    s0 = State::Fail;
                }
                else 
                    j = 0;
				for (; j < 7; j++) {
					if (i + j < line.size() && cmp[j] == line[i + j]) {
						temp += cmp[j];
						continue;
					}
					outputF << "#" << temp;
					if (i + j < line.size())
						i = maxx(i + j - 1, 0);
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
					if (FileExists(directory + sep + temp0)) {
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
        if (!noBreak)
		    outputF << "\n";
	}
	included.insert(std::make_pair(filename, 1));
	inp.close();
}

void GetDirFiles(const std::string &directory, std::vector<std::string>* vec) {
    NWFile file;
    int len;
    if (!GetFirstFile(&file, directory.c_str())) {
        return; 
    }
#define check(n,c) file.name[len-n] == c 
    do {
        if (file.isDir)
            continue;
        len = strlen(file.name);
        if (len > 3 && check(4,'.') && check(1,'p') && check(2,'p') && check(3, 'h')){
            vec->push_back(file.name);
            continue;
        }
        if (len > 2 && check(2,'.') && check(1,'h')){
            vec->push_back(file.name);
            continue;
        }
    } while (GetNextFile(&file));
#undef check
}

void AssembleHeaders(const std::string& src, const std::string& dst, const std::string& outputName, const std::string& first) {
	std::vector<std::string> vec;
    GetDirFiles(src, &vec);
    if (!first.empty()) {
        int index = -1;
	    for (int i = 0; i < vec.size(); ++i) {
            if (vec[i] != first) continue;
            index = i; 
            break;
        }
        if (index == -1) {
            printf("\"first\" file not found, it is ignored\n");
        }
        else {
            std::swap(vec[0], vec[index]);
        }
    }
	outputF.open(dst + sep + outputName);
	if (!outputF) {
		printf("Error openning dst file, check existence of output folder\n");
		outputF.close();
		return;
	}
	outputF << "#pragma once\n";
	for (std::string& file : vec) {
		if (included.find(file) != included.end())
			continue;
		Inc(src, file);
	}
	outputF.close();
}
