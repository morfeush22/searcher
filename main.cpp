#include <iostream>
#include <experimental/filesystem>
#include <utility>

#include "Searcher.h"
#include "Printer.h"
#include "PrintSearchTask.h"

namespace fs = std::experimental::filesystem;
using namespace std;


int main(int argv, char *argc[]) {
	Printer printer;
	fs::directory_options opt = fs::directory_options::follow_directory_symlink | fs::directory_options::skip_permission_denied;
	for(auto& p: fs::recursive_directory_iterator("a", opt)) {
		if (fs::is_regular_file(p)) {
			//std::cout << p << '\n';

			PrintSearchTask pst {printer, Searcher{p.path(), "dupasraka"}, p.path()};
			pst();
			//std::string s;
			//std::string path = p.path();
			//while(!(s = fh.Search()).empty()) {
			//	printer.Print(path, s);
			//}
		}
	}
	return 0;
}
