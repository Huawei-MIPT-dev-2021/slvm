#include "utils/filemap.h"
#include "utils/common.h"

extern "C" {
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
}

namespace util
{
std::optional<FileMap> FileMap::Create(char const *path, FileMap::Mode mode)
{
	int omode, prot, fd;
	void *mem;
	struct stat fst;

	switch (mode) {
	case FileMap::RDONLY:
		omode = O_RDONLY;
		prot = PROT_READ;
		break;
	case FileMap::RDWR:
		omode = O_RDWR;
		prot = PROT_READ | PROT_WRITE;
		break;
	default:
		UNREACHABLE();
	}

	if ((fd = open(path, omode)) < 0)
		return {};

	if (fstat(fd, &fst) < 0)
		goto out_close;

	if ((mem = mmap(NULL, fst.st_size, prot, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		goto out_close;

	return FileMap(fd, (u8 *)mem, fst.st_size);
out_close:
	close(fd);
	return {};
}

FileMap::~FileMap()
{
	if (!mem)
		return;
	munmap(mem, size);
	close(fd);
}
} // namespace util