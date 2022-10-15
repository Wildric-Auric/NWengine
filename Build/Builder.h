#include <vector>
#include <vector>
#include <string>

class Builder {
public:
	static void Compile();
	static void Link();
	static void InitScripts();
	static void Build();
	static std::vector<std::string> PreprocessorMacros;
};