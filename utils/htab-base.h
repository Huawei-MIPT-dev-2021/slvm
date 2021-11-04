#pragma once

#include "utils/alloc.h"
#include "utils/common.h"

namespace util
{
/* Flexible hashtable template */
template <typename E, typename H, u8 bits = 8>
class HtabBase
{
public:
	struct Node {
	public:
		Node *next{nullptr};
		template <typename K>
		Node(K &k) : e(k)
		{
		}
		E e;
	};

	template <typename A, typename K = E>
	inline E &access(K &k, A allocfn)
	{
		H hashfn;
		size_t hash = (size_t)hashfn(k) & (nBuckets - 1);
		auto *n = buckets[hash];
		for (; n && !(n->e == k); n = n->next)
			;
		if (!n)
			n = new (allocfn(k)) Node(k);
		return n->e;
	}

	template <typename F>
	inline void clear(F freefn)
	{
		for (size_t b = 0; b < nBuckets; ++b) {
			auto n = buckets[b];
			while (n) {
				auto next = n->next;
				freefn(n);
				n = next;
			}
		}
	}

private:
	static constexpr size_t nBuckets = (size_t)1 << bits;
	Node *buckets[nBuckets] = {}; // in place
};
} // namespace util