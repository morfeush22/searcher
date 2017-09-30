#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <fstream>
#include <memory>
#include <cctype>

class FileReader {
public:
	FileReader(const std::string &path, std::size_t prefix_size, std::size_t suffix_size, std::size_t max_str_size,
			std::size_t buffer_size = 1024);
	virtual ~FileReader();

	FileReader(FileReader &&other) = default;
	FileReader & operator=(FileReader &&other) = default;

	char * begin();
	char * end();
	const char * cbegin();
	const char * cend();

	auto file_pos() const noexcept {
		return file_pos_;
	}

	bool valid() const noexcept {
		return (hook_.rdstate() & (std::ifstream::failbit | std::ifstream::badbit)) == 0;
	}

	/*
	std::string path() const noexcept {
		return path_;
	}
	*/

	bool Read();

	enum class OffsetPosition {
		ENTIRE_GUARDED,
		PARTLY_GUARDED,
		UNGUARDED
	};

	OffsetPosition CheckPositionOfOffset(std::size_t offset);

private:
	auto SizeLeft() const noexcept {
		return file_size_ - size_read_;
	}

	auto IsLargerThanUnguarded(std::size_t size) const noexcept {
		return size > kUnguardedSize;
	}

	auto IsLargerThanBuffer(std::size_t size) const noexcept {
		return size > kBufferSize;
	}

	auto DataSizeInGuarded() const noexcept {
		return c_buffer_len_ - c_buffer_len_unsafe_;
	}

	std::size_t ReadFirstChunk();
	std::size_t  ReadNextChunk();

	std::string path_;
	std::ifstream hook_;

	const std::size_t kBufferSize;
	const std::size_t kPrefixSize;
	const std::size_t kSuffixSize;
	const std::size_t kMaxStrSize;
	const std::size_t kGuardSize;
	const std::size_t kUnguardedSize;

	std::size_t file_size_ = 0;
	std::size_t file_pos_ = 0;

	std::unique_ptr<char []> buffer_;

	bool first_read_ = true;

	std::size_t size_read_ = 0;
	std::size_t c_buffer_len_ = 0;
	std::size_t c_buffer_len_unsafe_ = 0;
};

#endif /* FILEREADER_H_ */
