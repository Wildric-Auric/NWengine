#include "Builder.h"
#include <iostream>

#define HELP_LOG "/C : Compile engine and scripts, will generate Object files\n/B : Link existing objs, generates an exe\n/IS: Include users scripts in the code"
#define UNK_LOG "Unknown command, try /Help"

int main(int argc, const char* argv[]) {
	if (argc <= 1) return 0;
	std::string cur = "";
	for (int i = 1; i < argc; i++) {
		cur = argv[i];
		if (cur == "/C") Builder::Compile();
		else if (cur == "/B") Builder::Link();
		else if (cur == "/IS") Builder::InitScripts();
		else if (cur == "/Help") std::cout << HELP_LOG << std::endl;
		else if ((cur.length() > 2) && (cur[0] == '/') && (cur[1] == 'M'))
			Builder::PreprocessorMacros.push_back(cur.substr(2, cur.length() - 2));
		else std::cout << cur << ": " << UNK_LOG << std::endl;
	}
	return 0;
}