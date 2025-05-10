#include <vector>
#include <string>



//Code borrowed from NWengine utilities
bool FileExists(std::string dir);
std::string GetFileName(std::string path, std::string* bFilename = nullptr, std::string* bExtension = nullptr, std::string* bRoot = nullptr);
std::vector<std::string> GetDirFiles(const std::string& directory, const std::string& extensionFilter = "");

void Inc(const std::string& directory, const std::string& filename);
void AssembleHeaders(const std::string& src, const std::string& dst, const std::string& outputName, const std::string& first);
