#pragma once

#include "utils/common.h"

namespace util
{
void *safemalloc(size_t sz);

class Allocator
{
public:
	virtual void *Alloc(size_t) = 0;
	virtual void Free(void *) = 0;
};

class Ralloc final : public Allocator
{
public:
	Ralloc();
	~Ralloc();

	void *Alloc(size_t sz) override;
	void Free(void *) override {}

private:
	struct Block {
		static constexpr size_t defaultSize = 64 * 1024;
		Block *next{nullptr};
		u8 *cur{nullptr};
		u8 *end{nullptr};
	};
	Block *head{nullptr};
};
} // namespace util