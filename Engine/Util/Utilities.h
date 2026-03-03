#pragma once
#include <string>
#include <vector>
#ifndef NOMINMAX
#define NOMINMAX
#endif

bool GetEnvVar(const char*, std::string*);

// TODO::use IDWriteFontCollection to get fonts correctly
void GetSystemFontDir(std::string* out);

void GetSystemFontDirALT(std::string* out);
/**
 * @brief Process text to make it compatible with lpstr filter in functions such as GetFile() or SaveAs
 *
 * @param filesType The type of files to filter (e.g., "Image Files", "Sound files")
 * @param types The file extensions separated by semicolon and preceded by a star (e.g., "*.jpeg;*.png;*.jpg")
 * @return The lpstr filter string
 */
#define WIN_STR_FILTER(filesType, types) filesType "\0" types "\0All\0*.*\0"

/**
 * @brief Extend the vector 'a' by appending the elements from vector 'b'
 *
 * @tparam T The type of elements in the vectors
 * @param a The vector to be extended
 * @param b The vector containing elements to be appended
 */
template <typename T> void ExtendVector(std::vector<T>* a, const std::vector<T>& b) {
	for(auto& c : b) {
		a->push_back(c);
	}
}

// Next functions use windows api; LATETODO:: Add their equivalent for mac, linux etc...
// #ifdef WIN

/*See Interface.cpp to know these functions role. I could have used only one loop to get number and string but it's cleaner when
 * done like like this and think it won't affect too much engine performance.
 */

/**
 * @brief A handle to a dynamic-link library (DLL)
 *
 */
class DllHandle {
  public:
	/**
	 * @brief Construct a new DllHandle object and load the specified DLL
	 *
	 * @param filename The name of the DLL file
	 */
	DllHandle() {};
	void Load(const char* filename);
	void Free();

	void* Get();
	void* h = nullptr;
};

/**
 * @brief Get the pointer to the specified function in the DLL
 *
 * @param dll The handle to the DLL
 * @param functionName The name of the function
 * @return The pointer to the specified function
 */
void* GetDllFunction(DllHandle* dll, const char* functionName);
/**
 * @brief Get the list of files in the specified directory
 *
 * @param directory The directory path
 * @param extensionFilter The file extension filter (optional)
 * @return A vector of strings representing the files in the directory
 */
std::vector<std::string> GetDirFiles(const std::string& directory, const std::string& extensionFilter = "");

/**
 * @brief Get the current working directory
 *
 * @return The current working directory
 */
std::string GetCurrentDir();

/**
 * @brief Get the path of the executable file
 *
 * @return The path of the executable file
 */
std::string GetExePath();
std::string GetFile(const char* type = "Text Files\0*.txt\0*.*\0");
std::string SaveAs(const char* type = "Text Files\0*.txt\0*.*\0");
std::string ToSingleBackSlash(const std::string& dir);
std::string ToDoubleBackSlash(const std::string& dir);
std::string GetFileName(const std::string& path, std::string* bFilename = nullptr, std::string* bExtension = nullptr, std::string* bRoot = nullptr);
bool CopyDirectory(const std::string& dest, const std::string& src);
bool MakeDir(const std::string& path);
bool MakeFile(const std::string& path);
bool FileExists(const std::string& dir);
bool Exec(const std::string& cmd, char* env = 0);
bool GetVcVarsEnv(std::vector<char>&);
bool FileDelete(const std::string& name);
bool FileMove(const std::string& dest, const std::string& source, bool failIfExists = false);
