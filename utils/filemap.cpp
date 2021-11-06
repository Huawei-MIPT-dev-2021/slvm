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
std::optional<FileMap> FileMap::Create(char const *path, FileMap::Mode mode, bool pad_zero)
{
	int omode, prot;
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

	int fd;
	if ((fd = open(path, omode)) < 0)
		return {};

	struct stat fst;
	if (fstat(fd, &fst) < 0)
		goto out_close;
	
	size_t map_sz;
	map_sz = fst.st_size + !!pad_zero;

	void *mem;
	if ((mem = mmap(NULL, map_sz, prot, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		goto out_close;

	if (pad_zero) {
		int page_sz = getpagesize();
		if ((map_sz - 1) % page_sz == 0) {
			void *lastpage = (u8*) mem + map_sz - 1;
			int rc = munmap(lastpage, page_sz);
			ASSERT(rc >= 0);
			void *remapped = mmap(lastpage, page_sz, prot, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, 0, 0);
			ASSERT(remapped == lastpage);
		}
	}

	return FileMap(fd, (u8 *)mem, map_sz);
out_close:
	close(fd);
	return {};
}

FileMap::~FileMap()
{
	if (!mem)
		return;
	int rc = munmap(mem, size);
	ASSERT(rc >= 0);
	close(fd);
}
} // namespace util