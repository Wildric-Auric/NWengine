#include "HeaderAssembler.h"

int main(int argc, const char* argv[]) {
    const char* first = 0;
	if (argc <= 1) {
		printf("arg0 : source directory\narg1 : output directory\narg2 : output filename\narg3(optional) : first file");
		return -1;
	}
	if (argc < 4) {
		printf("You should Provide all required arguments");
        exit(-1);
	}
    if (argc >= 5) {
        first = argv[4];
    }

	AssembleHeaders(argv[1], argv[2], argv[3], first);
}
