#ifndef PRINTSEARCHTASK_H_
#define PRINTSEARCHTASK_H_

#include "Searcher.h"
#include "Printer.h"
#include <string>
#include <utility>

class PrintSearchTask {
public:
	PrintSearchTask(const std::unique_ptr<Printer> &printer, std::unique_ptr<Searcher> searcher);
	virtual ~PrintSearchTask() = default;

	PrintSearchTask(PrintSearchTask &&other):
		printer_{other.printer_},
		searcher_{std::move(other.searcher_)}
		//path_{std::move(other.path_)}
	{
	}

	void operator()();

private:
	const std::unique_ptr<Printer> &printer_;
	std::unique_ptr<Searcher> searcher_;
	//const std::string path_;
};

#endif /* PRINTSEARCHTASK_H_ */
