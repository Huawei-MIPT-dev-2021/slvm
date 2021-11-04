#include "utils/filemap.h"
#include "utils/strtab.h"
#include <iostream>

int main(int argc, char **argv)
{
	ASSERT(argc == 2);
	auto fmap = util::FileMap::Create(argv[1], util::FileMap::RDONLY);
	if (!fmap.has_value()) {
		return 1;
	}

	util::StrTab strtab;

	char *c = (char *)fmap->GetMem();
	std::cout << strtab.UniqueStr(c);

	return 0;
}