#pragma once

#include "utils/alloc.h"
#include "utils/strtab.h"
#include <ostream>

namespace cjlcc
{
class Lex
{
public:
	struct Tok {
		enum Kind {
#define DEF_TOKEN(name, print) name,
#include "cjlcc/inc/tokens.inc"
#undef DEF_TOKEN
		} kind;
		union Value {
			u16 c;
			i64 i;
			double f;
			char const *s;
			char const *id;
		} val;
	};
	inline const Tok &Next()
	{
		ParseTok();
		return tok;
	}
	inline const Tok &Cur()
	{
		return tok;
	}

	Lex(char const *in_data_, size_t in_size_, util::StrTab &strtab_, util::Ralloc &str_arena_)
	    : in_beg(in_data_), in_end(in_data_ + in_size_), strtab(strtab_), str_arena(str_arena_)
	{
		cp = in_data_;
		line = cp;
		lineno = 1;
	}

private:
	inline void SkipWS();

	inline u16 GetWordLen();
	inline bool GetKword(u16);
	inline void GetId(u16);
	
	inline bool GetStrLit();
	inline bool GetStrLitChr(char &res);

	inline bool GetIntLit();

	inline bool GetPunct();
	void ParseTok();

	static Tok tok;

	char const *const in_beg;
	char const *const in_end;
	util::StrTab &strtab;
	util::Ralloc &str_arena;

	char const *cp;
	char const *line;
	size_t lineno;
};

std::ostream &operator<<(std::ostream &os, Lex::Tok const &t);
} // namespace cjlcc