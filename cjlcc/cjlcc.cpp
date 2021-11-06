#include "cjlcc/lex.h"
#include "utils/alloc.h"
#include "utils/filemap.h"
#include "utils/strtab.h"
#include <iostream>

namespace cjlcc
{
int main(int argc, char **argv)
{
	ASSERT(argc == 2);
	auto fmap = util::FileMap::Create(argv[1], util::FileMap::RDONLY, true);
	if (!fmap.has_value()) {
		return 1;
	}

	util::StrTab strtab;
	util::Ralloc str_arena;

	cjlcc::Lex lex((char *)fmap->GetMem(), fmap->GetSize(), strtab, str_arena);
	while (1) {
		auto &tok = lex.Next();
		std::cout << tok << "\n";
		if (tok.kind == Lex::Tok::END)
			break;
	}
	std::cout << std::endl;

	return 0;
}
} // namespace cjlcc

int main(int argc, char **argv)
{
	return cjlcc::main(argc, argv);
}