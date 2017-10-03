#include "SearcherFactory.h"

std::unique_ptr<Searcher> SearcherFactory::Create(const std::string &path, const std::string &str, std::size_t prefix_size, std::size_t suffix_size, std::size_t max_str_size) {
	FileReader fr{path, prefix_size, suffix_size, max_str_size};
	std::unique_ptr<Searcher> searcher = std::make_unique<Searcher>(str, std::move(fr), prefix_size, suffix_size);
	return searcher;
}
