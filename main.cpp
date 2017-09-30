#include <iostream>
#include <experimental/filesystem>
#include <utility>
#include <future>
#include <queue>
#include <type_traits>

#include "Searcher.h"
#include "Printer.h"
#include "PrintSearchTask.h"

namespace fs = std::experimental::filesystem;
using namespace std;


int main(int argv, char *argc[]) {
	std::queue<std::future<void>> q;
	Printer printer;
	fs::directory_options opt = fs::directory_options::follow_directory_symlink | fs::directory_options::skip_permission_denied;
	for(auto& p: fs::recursive_directory_iterator("a", opt)) {
		if (fs::is_regular_file(p)) {
			PrintSearchTask pst {printer, Searcher{p.path(), "dupasraka"}, p.path()};
			auto f = std::async(std::move(pst));
			q.push(std::move(f));
		}
	}
	while(!q.empty()) {
		auto f = std::move(q.front());
		q.pop();
		f.get();
	}
	return 0;
}
