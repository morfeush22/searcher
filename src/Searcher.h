#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "FileReader.h"

#include <fstream>
#include <memory>
#include <cctype>
#include <utility>


class Searcher {
public:
	Searcher(const std::string &str, FileReader &&file_reader, std::size_t prefix_size, std::size_t suffix_size);

	Searcher(Searcher &&other) = default;
	Searcher & operator=(Searcher &&other) = default;
	virtual ~Searcher() = default;

	std::string Search();
	const std::string & FilePath() const {
		return file_reader_.path();
	}

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

	FileReader file_reader_;
	const std::string str_;
	bool skip_ = false;
	const char * itr_ = nullptr;
};

#endif /* SEARCHER_H_ */
