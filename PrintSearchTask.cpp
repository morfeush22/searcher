#include "PrintSearchTask.h"
#include <utility>

PrintSearchTask::PrintSearchTask(const std::unique_ptr<Printer> &printer, std::unique_ptr<Searcher> searcher):
printer_{printer},
searcher_{std::move(searcher)}
//path_{path}
{
}

void PrintSearchTask::operator ()() {
	std::string str;
	while(!(str = searcher_->Search()).empty()) {
		printer_->Print(searcher_->FilePath(), str);
	}
}
