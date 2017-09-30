#ifndef PRINTSEARCHTASK_H_
#define PRINTSEARCHTASK_H_

#include "Searcher.h"
#include "Printer.h"
#include <string>

class PrintSearchTask {
public:
	PrintSearchTask(const Printer &printer, Searcher &&searcher, const std::string &path);
	virtual ~PrintSearchTask() = default;

	void operator()();

private:
	const Printer &printer_;
	Searcher searcher_;
	const std::string path_;
};

#endif /* PRINTSEARCHTASK_H_ */
