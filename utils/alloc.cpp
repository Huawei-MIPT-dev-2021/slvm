#include "utils/alloc.h"
#include <cstdlib>
#include <algorithm>

namespace util
{
union _minaligned {
	size_t a;
	double b;
	void *c;
	void (*d)();
};
constexpr size_t minalign = sizeof(_minaligned);

void *safemalloc(size_t sz)
{
	void *mem = malloc(sz);
	if (!mem)
		abort();
	return mem;
}

Ralloc::Ralloc() {}

Ralloc::~Ralloc()
{
	while (head) {
		Block *next = head->next;
		free(head);
		head = next;
	}
}

void *Ralloc::Alloc(size_t sz)
{
	Ralloc::Block *blk;
	sz = roundup(sz, minalign);

	if (!(head && (sz <= head->end - head->cur))) {
		size_t blk_sz = std::max(Block::defaultSize, sz);
		blk_sz += roundup(sizeof(Block), minalign);
		blk_sz = next_pow2_64(blk_sz);
		blk = static_cast<Block *>(safemalloc(blk_sz));
		blk->cur = (u8 *)blk + roundup(sizeof(Block), minalign);
		blk->end = (u8 *)blk + blk_sz;

		blk->next = head;
		head = blk;
	}
	blk->cur += sz;
	return blk->cur - sz;
}

} // namespace util