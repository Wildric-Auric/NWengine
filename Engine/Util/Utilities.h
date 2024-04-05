#pragma once
#include <string>
#include <vector>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN 

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
template<typename T> 
void ExtendVector(std::vector<T>* a, const std::vector<T>& b) {
    for (auto& c : b) {
        a->push_back(c);
    }
}

/**
 * @brief Get a list of files and directories in the specified path
 * 
 * @param path The path to the directory
 * @return A vector of strings representing the files and directories in the path
 */
std::vector<std::string> GetNWlist(std::string path);

//Next functions use windows api; LATETODO:: Add their equivalent for mac, linux etc...
//#ifdef WIN

/*See Interface.cpp to know these functions role. I could have used only one loop to get number and string but it's cleaner when done like
* like this and think it won't affect too much engine performance.
*/

/**
 * @brief A handle to a dynamic-link library (DLL)
 * 
 */
struct DllHandle {
    /**
     * @brief Construct a new DllHandle object and load the specified DLL
     * 
     * @param filename The name of the DLL file
     */
    DllHandle(const char* filename);
    DllHandle() {};
    ~DllHandle();

    /**
     * @brief Get the pointer to the loaded DLL
     * 
     * @return The pointer to the loaded DLL
     */
    const void* Get() const;

private:
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
 * @brief Get the number of files in the specified directory and its subdirectories recursively
 * 
 * @param directory The directory path
 * @return A vector of integers representing the number of files in each subdirectory
 */
std::vector<int> GetRecusivelyFilesNumber(const std::string& directory);

/**
 * @brief Get the list of files in the specified directory
 * 
 * @param directory The directory path
 * @param extensionFilter The file extension filter (optional)
 * @return A vector of strings representing the files in the directory
 */
std::vector<std::string> GetDirFiles(const std::string& directory, const std::string& extensionFilter = "");

/**
 * @brief Get the list of files in the specified directory and its subdirectories recursively
 * 
 * @param directory The directory path
 * @return A vector of strings representing the files in the directory and its subdirectories
 */
std::vector<std::string> GetRecusivelyDirFiles(const std::string& directory);

/**
 * @brief Accumulate the elements of vector 'b' into vector 'a' starting from the specified index
 * 
 * @param a The destination vector
 * @param b The source vector
 * @param index The starting index in vector 'a' (default is 0)
 * @return The accumulated value
 */
int AccumulateChildren(std::vector<int>* a, std::vector<int>* b, int index = 0);

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

/**
 * @brief Get the path of a selected file using a file dialog
 * 
 * @param type The type of files to filter (default is "Text Files\0*.txt\0*.*\0")
 * @return The path of the selected file
 */
std::string GetFile(const char* type = "Text Files\0*.txt\0*.*\0");

/**
 * @brief Save the current content to a file using a file dialog
 * 
 * @param type The type of files to filter (default is "Text Files\0*.txt\0*.*\0")
 * @return The path of the saved file
 */
std::string SaveAs(const char* type = "Text Files\0*.txt\0*.*\0");

/**
 * @brief Convert all single backslashes in the string to double backslashes
 * 
 * @param dir The input string
 * @return The string with all single backslashes converted to double backslashes
 */
std::string ToSingleBackSlash(const std::string& dir);

/**
 * @brief Convert all double backslashes in the string to single backslashes
 * 
 * @param dir The input string
 * @return The string with all double backslashes converted to single backslashes
 */
std::string ToDoubleBackSlash(const std::string& dir);

/**
 * @brief Get the filename and extension from the specified path
 * 
 * @param path The file path
 * @param bFilename The output parameter to store the filename (optional)
 * @param bExtension The output parameter to store the file extension (optional)
 * @param bRoot The output parameter to store the root directory (optional)
 * @return The filename and extension
 */
std::string GetFileName(std::string path, std::string* bFilename = nullptr, std::string* bExtension = nullptr, std::string* bRoot = nullptr);

/**
 * @brief Copy a directory and its contents to the specified destination
 * 
 * @param dest The destination directory path
 * @param src The source directory path
 * @return True if the copy operation is successful, false otherwise
 */
bool CopyDirectory(const std::string& dest, const std::string& src);

/**
 * @brief Create a directory at the specified path
 * 
 * @param path The directory path
 * @return True if the directory is created successfully, false otherwise
 */
bool MakeDir(const std::string& path);

/**
 * @brief Create a file at the specified path
 * 
 * @param path The file path
 * @return True if the file is created successfully, false otherwise
 */
bool MakeFile(const std::string& path);

/**
 * @brief Copy a file from the source path to the destination path
 * 
 * @param dest The destination file path
 * @param src The source file path
 * @param failIfExists Flag indicating whether to fail if the destination file already exists (default is false)
 * @return True if the copy operation is successful, false otherwise
 */
bool FileCopy(std::string dest, std::string src, bool failIfExists = false);

/**
 * @brief Check if a file exists at the specified path
 * 
 * @param dir The file path
 * @return True if the file exists, false otherwise
 */
bool FileExists(std::string dir);

/**
 * @brief Execute a command in the system shell
 * 
 * @param cmd The command to execute
 * @return True if the command is executed successfully, false otherwise
 */
bool Exec(std::string cmd);

/**
 * @brief Delete a file at the specified path
 * 
 * @param name The file path
 * @return True if the file is deleted successfully, false otherwise
 */
bool FileDelete(std::string name);

/**
 * @brief Move a file from the source path to the destination path
 * 
 * @param dest The destination file path
 * @param source The source file path
 * @param failIfExists Flag indicating whether to fail if the destination file already exists (default is false)
 * @return True if the move operation is successful, false otherwise
 */
bool FileMove(std::string dest, std::string source, bool failIfExists = false);