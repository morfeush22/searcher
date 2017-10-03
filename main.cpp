#include <iostream>
#include <experimental/filesystem>
#include <utility>
#include <future>
#include <queue>
#include <type_traits>

#include "Searcher.h"
#include "Printer.h"
#include "PrintSearchTask.h"
#include "SearcherFactory.h"

namespace fs = std::experimental::filesystem;
using namespace std;

void SearchAndPrint(const std::string &path, const std::string &str) {
	fs::directory_options opts = fs::directory_options::follow_directory_symlink | fs::directory_options::skip_permission_denied;
	std::unique_ptr<SearcherFactory> factory = std::make_unique<SearcherFactory>();
	std::unique_ptr<Printer> printer = std::make_unique<Printer>();
	std::queue<std::future<void>> queue;

	for(auto &p: fs::recursive_directory_iterator(path, opts)) {
		if (fs::is_regular_file(p)) {
			std::unique_ptr<Searcher> searcher = factory->Create(p.path(), str);
			PrintSearchTask pst {printer, std::move(searcher)};
			auto f = std::async(std::move(pst));
			queue.push(std::move(f));
		}
	}

	while(!queue.empty()) {
		auto task = std::move(queue.front());
		queue.pop();
		task.get();
	}
}

int main(int argv, char *argc[]) {
	SearchAndPrint("a", "dupasraka");
	return 0;
}
