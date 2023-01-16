#include <vector>
#include <string>

class Builder {
private:
	static void GenerateCompilationData(std::ofstream* ofs, std::string file, std::string outPutData);
	static void InitCompilation(std::ofstream* ofs, std::string outputDir);
public:
	static void Compile(std::string outputDir);
	static void Link(std::string output); //Link all objs in a dir
	static void InitScripts(std::string scriptList, std::string scriptManager, std::string output);
	static void Build();
	static void CompileInd(std::string file, std::string outputDir); //Compile the cpp file passed to .obj
	static void GenLib(std::string output);
	static std::vector<std::string> GetDirs(std::string path);

	static std::string runtimeLib;
	static std::vector<std::string> PreprocessorMacros;
	static std::vector<std::string> LibsDir;
	static std::vector<std::string> IncludeDir;
	static std::vector<std::string> staticLibs;
	static std::vector<std::string> objs;
	
};