#ifndef SEARCHERFACTORY_H_
#define SEARCHERFACTORY_H_

#include "Searcher.h"
#include "FileReader.h"
#include <memory>
#include <cctype>

class SearcherFactory {
public:
	SearcherFactory();
	virtual ~SearcherFactory();

	virtual std::unique_ptr<Searcher> Create(const std::string &path, const std::string &str, std::size_t prefix_size = 3, std::size_t suffix_size = 3, std::size_t max_str_size = 128);
};

#endif /* SEARCHERFACTORY_H_ */
