#ifndef PRINTSEARCHTASK_H_
#define PRINTSEARCHTASK_H_

#include "Searcher.h"
#include "Printer.h"
#include <string>
#include <utility>

class PrintSearchTask {
public:
	PrintSearchTask(const Printer &printer, Searcher &&searcher, const std::string &path);
	virtual ~PrintSearchTask() = default;

	PrintSearchTask(PrintSearchTask &&other):
		printer_{other.printer_},
		searcher_{std::move(other.searcher_)},
		path_{std::move(other.path_)}
	{
	}

	void operator()();

private:
	const Printer &printer_;
	Searcher searcher_;
	const std::string path_;
};

#endif /* PRINTSEARCHTASK_H_ */
