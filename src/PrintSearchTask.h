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

	PrintSearchTask(PrintSearchTask &&other);

	void operator()();

private:
	const std::unique_ptr<Printer> &printer_;
	std::unique_ptr<Searcher> searcher_;
};

#endif /* PRINTSEARCHTASK_H_ */
