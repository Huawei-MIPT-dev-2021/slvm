#include "strtab.h"

namespace util
{
char const *StrTab::UniqueStr(char const *str)
{
	StrInfo s = {.len = (u16)strlen(str), .str = (u8 *)str};
	auto alloc = [this](StrInfo &s) { return arena.Alloc(s.ElemSize()); }; // use arena
	auto &e = htab.access(s, alloc);
	return reinterpret_cast<char const *>(e.str);
}
} // namespace util