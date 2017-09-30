#include "Searcher.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <utility>


Searcher::Searcher(const std::string &path, const std::string &str,
		std::size_t prefix_size, std::size_t suffix_size, std::size_t max_str_size):
kPrefixSize{prefix_size},
kSuffixSize{suffix_size},
kMaxStrSize{max_str_size},
file_reader_{path, prefix_size, suffix_size, max_str_size}
{
	str_ = str;	//TODO verification, maybe factory method
}

void Searcher::ComposeString(std::size_t offset, std::string *str) {
	std::size_t str_size = str_.size();
	std::size_t buffer_size = file_reader_.cend() - file_reader_.cbegin();

	auto prefix_size = offset > kPrefixSize ? kPrefixSize : offset;
	auto suffix_size = (buffer_size - offset - str_size) > kSuffixSize ? kSuffixSize : buffer_size - offset - str_size;

	str->assign(itr_ - prefix_size, prefix_size + str_size + suffix_size);

	std::stringstream ss;
	ss << "(" << file_reader_.file_pos() + offset << ")";

	str->insert(0, ss.str());
}

auto Searcher::SelectPositionDependentAction(FileReader::OffsetPosition pos) {
	if (pos ==  FileReader::OffsetPosition::ENTIRE_GUARDED) {
		return WILL_RETURN_ON_NEXT_SEGMENT;
	}
	else if (skip_ && pos == FileReader::OffsetPosition::UNGUARDED) {
		return SKIP_ON_THIS_ITERATION;
	}
	else {
		return SAVE_AND_ADVANCE;
	}
}

void Searcher::ReadFirstSegment() {
	file_reader_.Read();
	itr_ = search(file_reader_.cbegin(), file_reader_.cend(), str_.begin(), str_.end());
}

std::string Searcher::Search() {
	if (!file_reader_.valid()) {
		return {};
	}

	if (itr_ == nullptr) {
		ReadFirstSegment();
	}

	// load next segments till string occurence found or end of file
	for (; itr_ == file_reader_.cend();) {
		if (!file_reader_.Read()) {
			return {};
		}
		itr_ = search(file_reader_.cbegin(), file_reader_.cend(), str_.begin(), str_.end());
	}

	// here we are sure, that string was found
	std::string cmpsd_str;
	auto off = itr_ - file_reader_.cbegin();
	auto pos = file_reader_.CheckPositionOfOffset(off);

	switch (SelectPositionDependentAction(pos)) {
	case WILL_RETURN_ON_NEXT_SEGMENT:
		itr_ = file_reader_.cend();
		return Search();
	case SKIP_ON_THIS_ITERATION:
		skip_ = false;
		++itr_;
		return Search();
	case SAVE_AND_ADVANCE:
		ComposeString(off, &cmpsd_str);
		if (pos == FileReader::OffsetPosition::PARTLY_GUARDED) {
			skip_ = true;
		}
		// advance to next string occurence or segment end
		itr_ = search(++itr_, file_reader_.cend(), str_.begin(), str_.end());
	}

	return cmpsd_str;
}
