#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "FileReader.h"

#include <fstream>
#include <memory>
#include <cctype>
#include <utility>


class Searcher {
public:
	Searcher(const std::string &path, const std::string &str,
			std::size_t prefix_size = 3, std::size_t suffix_size = 3, std::size_t max_str_size = 128);

	Searcher(Searcher &&other) = default;
	Searcher & operator=(Searcher &&other) = default;
	virtual ~Searcher() = default;

	std::string Search();

private:
	enum {
		WILL_RETURN_ON_NEXT_SEGMENT,
		SKIP_ON_THIS_ITERATION,
		SAVE_AND_ADVANCE
	};

	void ComposeString(std::size_t offset, std::string *str);
	void ReadFirstSegment();

	auto SelectPositionDependentAction(FileReader::OffsetPosition pos);

	const std::size_t kPrefixSize;
	const std::size_t kSuffixSize;
	const std::size_t kMaxStrSize;

	std::string str_;
	FileReader file_reader_;
	bool skip_ = false;
	const char * itr_ = nullptr;
};

#endif /* SEARCHER_H_ */
