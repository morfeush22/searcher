#include <iostream>
#include "FileHook.h"


int main(int argv, char *argc[]) {
	FileHook fh {"./example.txt", "dupasraka"};
	std::string s;
	while(!(s = fh.Search()).empty()) {
		std::cout << s << std::endl;
	}
	return 0;
}
