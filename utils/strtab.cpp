#include "strtab.h"

namespace util
{
char const *StrTab::UniqueStrN(char const *str, u16 len)
{
	StrInfo s = {.len = len, .str = (u8 *)str};
	auto alloc = [this](StrInfo &s) { return arena.Alloc(s.ElemSize()); };
	auto &e = htab.access(s, alloc);
	return reinterpret_cast<char const *>(e.str);
}

char const *StrTab::UniqueStr(char const *str)
{
	return UniqueStrN(str, strlen(str));
}
} // namespace util