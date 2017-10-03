#include <iostream>
#include <experimental/filesystem>
#include <utility>
#include <future>
#include <queue>
#include <vector>

#include "Searcher.h"
#include "Printer.h"
#include "PrintSearchTask.h"
#include "SearcherFactory.h"

namespace fs = std::experimental::filesystem;
using namespace std;


void ShowUsage() {
    std::cerr << "Usage: searcher [OPTION]... SOURCE STRING\n"
              << "\n"
              << "\t-h, --help\t\tShow this help message\n"
              << std::endl;
}

template<typename Factory, typename Printer, typename Path, typename String>
decltype(auto) CreateTask(const Factory &factory, const Printer &printer, const Path &path, const String &str) {
	auto searcher = factory->Create(path, str);
	PrintSearchTask pst {printer, std::move(searcher)};
	return pst;
}

void SearchAndPrint(const std::string &path, const std::string &str) {
	if (!fs::exists(path)) {
		std::cerr << "file do not exist... " << path << "\n";
		return;
	}

	std::unique_ptr<SearcherFactory> factory = std::make_unique<SearcherFactory>();
	std::unique_ptr<Printer> printer = std::make_unique<Printer>();

	if (fs::is_regular_file(path)) {
		auto pst = CreateTask(factory, printer, path, str);
		pst();
	}
	else if (fs::is_directory(path)) {
		fs::directory_options opts = fs::directory_options::follow_directory_symlink | fs::directory_options::skip_permission_denied;
		std::queue<std::future<void>> queue;

		for(auto &p: fs::recursive_directory_iterator(path, opts)) {
			if (fs::is_regular_file(p)) {
				auto pst = CreateTask(factory, printer, p.path(), str);
				auto fut = std::async(std::move(pst));
				queue.push(std::move(fut));
			}
		}

		while(!queue.empty()) {
			auto task = std::move(queue.front());
			queue.pop();
			task.get();
		}
	}
	else {
		std::cerr << "wrong file... " << path << "\n";
		return;
	}
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
    	ShowUsage();
        return 1;
    }

    std::vector<std::string> args;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
        	ShowUsage();
            return 0;
        }
        else {
            args.push_back(argv[i]);
        }
    }

	SearchAndPrint(args[0], args[1]);

	return 0;
}
