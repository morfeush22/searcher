#include "FileReader.h"

#include <cstring>
#include <iostream>
#include <utility>

FileReader::FileReader(const std::string &path, std::size_t prefix_size, std::size_t suffix_size, std::size_t max_str_size,
		std::size_t buffer_size):
path_{path},
kBufferSize{buffer_size},
kPrefixSize{prefix_size},
kSuffixSize{suffix_size},
kMaxStrSize{max_str_size},
kGuardSize{prefix_size + max_str_size + suffix_size},
kUnguardedSize{kBufferSize - kGuardSize}
{
	hook_.open(path);
	if (!valid()) {
		std::cerr << "error opening file... " << path_ << "\n";
	}
	else {
		hook_.seekg(0, hook_.end);
		file_size_ = hook_.tellg();
		hook_.seekg(0, hook_.beg);

		buffer_ = std::make_unique<char []>(kBufferSize);
	}
}

FileReader::~FileReader() {
	if (hook_.is_open()) {
		try {
			hook_.close();
		}
		catch (...) {
			std::cerr << "error closing file... " << path_ << "\n";
		}
	}
}

const char * FileReader::cbegin() {
	return begin();
}

const char * FileReader::cend() {
	return end();
}

char * FileReader::begin() {
	return buffer_.get();
}

char * FileReader::end() {
	return begin() + c_buffer_len_;
}

bool FileReader::Read() {
	auto size = DataSizeInGuarded();

	std::size_t read;

	if (size) {
		char *buff = buffer_.get();
		std::memmove(buff, buff + kUnguardedSize, size);
		c_buffer_len_ = c_buffer_len_unsafe_ = size;
	}

	if (first_read_) {
		read = ReadFirstChunk();
		first_read_ = false;
	}
	else {
		file_pos_ += kUnguardedSize;
		read = ReadNextChunk();
	}

	return read || size;
}

std::size_t  FileReader::ReadFirstChunk() {
	auto size_left = SizeLeft();

	std::size_t size_to_read;

	if (!IsLargerThanUnguarded(size_left)) {
		size_to_read = c_buffer_len_ = c_buffer_len_unsafe_ = size_left;
	}
	else if (!IsLargerThanBuffer(size_left)) {
		size_to_read = c_buffer_len_ = size_left;
		c_buffer_len_unsafe_ = kUnguardedSize;
	}
	else {
		size_to_read = c_buffer_len_ = kBufferSize;
		c_buffer_len_unsafe_ = kUnguardedSize;
	}

	hook_.read(begin(), size_to_read);
	size_read_ += size_to_read;

	return size_to_read;
}

std::size_t  FileReader::ReadNextChunk() {
	auto size_left = SizeLeft();

	if (size_left == 0) {
		return 0;
	}

	size_t size_to_read;

	if (IsLargerThanUnguarded(size_left)) {
		size_to_read  = kUnguardedSize;
		hook_.read(begin() + kGuardSize, size_to_read);
		c_buffer_len_ = kBufferSize;
		c_buffer_len_unsafe_ = kUnguardedSize;
		size_read_ += size_to_read;
	}
	else {
		size_to_read = size_left;
		hook_.read(begin() + kGuardSize, size_to_read);
		c_buffer_len_ = kGuardSize + size_to_read;
		c_buffer_len_unsafe_ = (c_buffer_len_ > kUnguardedSize ? kUnguardedSize :  c_buffer_len_);
		size_read_ += size_to_read;
	}

	return size_to_read;
}

FileReader::OffsetPosition FileReader::CheckPositionOfOffset(std::size_t offset) {
	if (offset >= (kBufferSize - kMaxStrSize - kSuffixSize)) {
		return OffsetPosition::ENTIRE_GUARDED;
	}
	else if (offset >= (kBufferSize - kPrefixSize - kMaxStrSize - kSuffixSize)) {
		return OffsetPosition::PARTLY_GUARDED;
	}
	else {
		return OffsetPosition::UNGUARDED;
	}
}
