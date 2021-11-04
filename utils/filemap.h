#pragma once

#include "utils/common.h"
#include <optional>

namespace util
{
class FileMap
{
public:
	enum Mode {
		RDONLY,
		RDWR,
	};

	static std::optional<FileMap> Create(char const *path, Mode mode);

	u8 *GetMem()
	{
		return mem;
	}

	size_t GetSize()
	{
		return size;
	}

	~FileMap();

	FileMap& operator=(FileMap &x) = delete;
	FileMap& operator=(FileMap &&x) = delete;

	FileMap(FileMap&) = delete;
	FileMap(FileMap&& x)
	{
		new (this) FileMap(x.fd, x.mem, x.size);
		x.mem = nullptr;
	}

private:
	FileMap(int fd_, u8 *mem_, size_t size_) : fd(fd_), mem(mem_), size(size_) {}
	int fd;
	u8 *mem;
	size_t size;
};
} // namespace util