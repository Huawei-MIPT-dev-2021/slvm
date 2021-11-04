#pragma once

#include "utils/alloc.h"
#include "utils/common.h"
#include "utils/htab-base.h"

#include <cstring>

namespace util
{
class StrTab
{
public:
	char const *UniqueStr(char const *str);

private:
	struct StrInfo {
		u16 len;
		u8 *str;
		size_t ElemSize()
		{
			return sizeof(Elem) + len + 1;
		}
	};

	struct Elem {
		inline bool IsEq(u8 const *str_, u16 len_)
		{
			if (len != len_)
				return false;
			return !memcmp(str, str_, len);
		}
		inline bool operator==(Elem &r)
		{
			return IsEq(r.str, r.len);
		}
		inline bool operator==(StrInfo &r)
		{
			return IsEq(r.str, r.len);
		}
		Elem(StrInfo &s) : len(s.len)
		{
			memcpy(str, s.str, len);
			str[len] = 0;
		}
		u16 len;
		u8 str[]; // null-terminated
	};

	class DJB2Hash
	{
	public:
		size_t operator()(StrInfo &k)
		{
			u32 h = 5381;
			u8 const *s = k.str;
			u16 len = k.len;
			for (u8 c = *s; len; c = *++s, --len)
				h = h * 33 + c;
			return static_cast<size_t>(h);
		}
	};

	Ralloc arena;
	HtabBase<Elem, DJB2Hash, 9> htab;
};
} // namespace util