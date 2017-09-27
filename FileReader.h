#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <fstream>
#include <memory>
#include <cctype>

class FileReader {
public:
	FileReader(const std::string &path, std::size_t buffer_size = 1024,
			std::size_t prefix_size = 3, std::size_t suffix_size = 3, std::size_t max_str_size = 128);
	virtual ~FileReader();

	FileReader(FileReader &&other);
	FileReader & operator=(FileReader &&other);

	char * begin();
	char * end();
	const char * cbegin();
	const char * cend();

	auto file_pos() const noexcept {
		return file_pos_;
	}

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

	std::size_t size_read_ = 0;
	std::size_t c_buffer_len_ = 0;
	std::size_t c_buffer_len_unsafe_ = 0;
};

#endif /* FILEREADER_H_ */