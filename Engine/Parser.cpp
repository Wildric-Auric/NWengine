#include "Parser.h"
#include <fstream>

void Parser::Clear() {
	dataArray.clear();
	tree.clear();
}

void Parser::Parse(std::string path) {
	std::ifstream file(path);
	if (!file) {
		printf("ERROR::Parser getting wrong path"); //TODO::Print it on the NWengine console  LOG
		return;
	}
	for (std::string line; std::getline(file, line);) {
		std::string kv = "";
		static uint8 br = 0;
		for (char c : line) {
			if (c == '"') {
				br = br + 1;
				if (br == 2) br = 0;
				continue;
			}
			if (br != 0) {
				kv += c;
				continue;
			}
			if (!c || c == ' ' || c == '\n' || c == '\t') continue;
			if (c == ':') {
				keyStack.push_back(dataArray.size());
				keyLoc.push_back(dataArray.size());
				break;
			}
			kv += c;
		};

		if (kv.empty()) continue;

		if (kv == "end") {
			rawData.push_back(kv);
			keyStack.pop_back();
			continue;
		};

		dataArray.push_back(kv);
		rawData.push_back(kv);
		tree.push_back(-1);
		for (uint16 index : keyStack) tree[index] += 1;
	}
}