#include "Serializer.h"
#include "NWReflection.h"

#include<fstream>
#include<vector>


//TODO::Check if script is inheriting from Reflected class
void Serializer::GenerateMeta(std::string src, std::string dest) {
	std::fstream in;
	std::ofstream out;
	char c;
	in.open(src);
	if (!in) return;
	out.open(dest);
	if (!out) return;

	std::string word = "";
	uint8 state = 0;
	std::vector<std::string> memberStack;
	std::vector<std::tuple<std::string, std::string, std::string>> attribs; //type, name, value
	while (in >> std::noskipws >> c) {
		if ((c == ' ') || (c == '\n') || (c == '\t') || (c == ';')) {
			if (word == "SERIALIZED_ATTR") {
				state = 1;
				word  = "";
				continue;
			}
			
			if (word == "NW_SERIAL_DATA") {
				for (std::tuple<std::string, std::string, std::string> attrib : attribs)
					out << "this.reflectionData.insert(std::make_pair("
					    <<  "\"" << std::get<1>(attrib) << "\"" 
					    <<",std::make_tuple(0, 0, "
					    << "\"" <<std::get<0>(attrib) << "\"" <<  "," 
						<< "\"" << std::get<2>(attrib) << "\"" << ")); \n";

				state = 0;
				word = "";
				continue;
			}

			if ((state == 1) && (word != "=") && word.size() > 0)
				memberStack.push_back(word);

			if (c == ';') {
				switch (memberStack.size()) {
				case 2:
					attribs.push_back(std::make_tuple(memberStack[0], memberStack[1], std::string("")));
					break;
				case 3:
					attribs.push_back(std::make_tuple(memberStack[0], memberStack[1], memberStack[2]));
					break;
				default:
					break;
				}
				state = 0;
				memberStack.clear();
			}
			out << word << c;
			word = "";
			continue;
		}

		word += c;
	}
	in.close();
	out.close();
}

