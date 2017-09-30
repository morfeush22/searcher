#include "PrintSearchTask.h"
#include <utility>

PrintSearchTask::PrintSearchTask(const Printer &printer, Searcher &&searcher, const std::string &path):
printer_{printer},
searcher_{std::move(searcher)},
path_{path}
{
}

void PrintSearchTask::operator ()() {
	std::string str;
	while(!(str = searcher_.Search()).empty()) {
		printer_.Print(path_, str);
	}
}
