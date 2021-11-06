#include "lex.h"
#include "utils/alloc.h"
#include <cctype>
#include <cstdint>
#include <utility>

namespace cjlcc
{
Lex::Tok Lex::tok;

static struct {
	char const *str;
} str_repr_map[] = {
#define DEF_TOKEN(name, str) {str},
#include "cjlcc/inc/tokens.inc"
#undef DEF_TOKEN
};

std::ostream &operator<<(std::ostream &os, Lex::Tok const &t)
{
	auto str = str_repr_map[t.kind].str;
	if (str)
		return os << str;
	if (t.kind == Lex::Tok::ID)
		return os << "ID:" << t.val.id;
	if (t.kind == Lex::Tok::CONST_S)
		return os << "CONST_S:\"" << t.val.s << "\"";
	if (t.kind == Lex::Tok::CONST_I)
		return os << "CONST_I:" << t.val.i;
	ASSERT(!"unknown token");
}

// TODO: generate fsm-switch
static struct {
	char const *str;
	Lex::Tok::Kind kind;
	char len;
} punct_map[] = {
#define DEF_PUNCT(name, str) {str, Lex::Tok::Kind::name, sizeof(str) - 1},
#include "cjlcc/inc/puncts.inc"
#undef DEF_PUNCT
};

// TODO: generate fsm-switch
static struct {
	char const *str;
	enum Lex::Tok::Kind kind;
	char len;
} kword_map[] = {
#define DEF_KWORD(name, str) {str, Lex::Tok::Kind::name, sizeof(str) - 1},
#include "cjlcc/inc/kwords.inc"
#undef DEF_KWORD
};

inline u16 Lex::GetWordLen()
{
	char const *word = cp;
	char c = *word++;
	if (!(isalpha(c) || c == '_'))
		return 0;
	u16 i = 0;
	do {
		if (i++ == 128)
			ASSERT(!"too long identifier");
		c = *word++;
	} while (c == '_' || isalnum(c));
	return i;
}

inline bool Lex::GetKword(u16 len)
{
	for (int i = 0; i < ARRAY_SIZE(kword_map); ++i) {
		if (len != kword_map[i].len)
			continue;
		if (!memcmp(cp, kword_map[i].str, len)) {
			tok.kind = kword_map[i].kind;
			cp += len;
			return 1;
		}
	}
	return 0;
}

inline void Lex::GetId(u16 len)
{
	tok.kind = Tok::ID;
	tok.val.id = strtab.UniqueStrN(cp, len);
	cp += len;
	return;
}

inline bool Lex::GetStrLitChr(char &res)
{
	char c = *cp;

	if ((isgraph(c) && c != '\\') || (isblank(c) && c != '\n')) {
		res = c;
		cp++;
		return true;
	}

	if (c != '\\')
		return false;
	cp++;

	c = *cp++;
	switch (c) {
#define ESCSEQ(chr, val)                                                                           \
	case chr:                                                                                  \
		res = val;                                                                         \
		return true;
		ESCSEQ('\'', '\'');
		ESCSEQ('\"', '\"');
		ESCSEQ('\?', '\?');
		ESCSEQ('\\', '\\');
		ESCSEQ('a', '\a');
		ESCSEQ('b', '\b');
		ESCSEQ('f', '\f');
		ESCSEQ('n', '\n');
		ESCSEQ('r', '\r');
		ESCSEQ('t', '\t');
		ESCSEQ('v', '\v');
#undef ESCSEQ
	}
	ASSERT(!"TODO: \\0, \\x, \\u escape seq.");
}

inline bool Lex::GetStrLit()
{
	if (*cp != '\"')
		return false;
	cp++;
	static char buf[65 * 1024];
	u16 i = 0;
	while (*cp != '\"') {
		if (!GetStrLitChr(buf[i++])) {
			ASSERT(!"invalid string literal");
		}
		ASSERT(i < UINT16_MAX);
	}
	cp++;
	buf[i++] = 0;
	char *amem = (char *)str_arena.Alloc(i);
	memcpy(amem, buf, i);
	tok.kind = Tok::CONST_S;
	tok.val.s = amem;
	return true;
}

inline void Lex::SkipWS()
{
	while (std::isspace(*cp)) {
		if (*cp == '\n') {
			lineno++;
			line = cp + 1;
		}
		cp++;
	}
}

inline bool Lex::GetPunct()
{
	char buf[3] = {};
	for (int i = 0; i < ARRAY_SIZE(buf) && cp[i] != 0; ++i)
		buf[i] = cp[i];

	for (int i = 0; i < ARRAY_SIZE(punct_map); ++i) {
		char const *str = punct_map[i].str;
		char str_len = punct_map[i].len;
		int n = 0;
		while (1) {
			if (buf[n] != str[n])
				break;
			if (n++ == str_len - 1) {
				tok.kind = punct_map[i].kind;
				cp += str_len;
				return 1;
			}
		}
	}
	return 0;
}

inline bool Lex::GetIntLit()
{
	char *ep;
	errno = 0;
	static_assert(sizeof(long) == 8);
	i64 res = strtol(cp, &ep, 0);
	if (errno) {
		ASSERT(!"ill integer literal");
		return false;
	}
	if (ep == cp)
		return false;
	tok.kind = Tok::CONST_I;
	tok.val.i = res;
	cp = ep;
	return true;
}

void Lex::ParseTok()
{
	SkipWS();
	if (*cp == '\0') {
		tok.kind = Tok::END;
		return;
	}
	if (GetStrLit())
		return;
	if (GetPunct())
		return;
	if (GetIntLit())
		return;

	u16 word_len = GetWordLen();
	if (word_len) {
		if (GetKword(word_len))
			return;
		GetId(word_len);
		return;
	}

	ASSERT(!"unrecognized token");
}
}; // namespace cjlcc