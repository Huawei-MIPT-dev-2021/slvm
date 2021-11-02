#include "utils/utils.h"
#include <iostream>

int main(int argc, char **argv)
{
	ASSERT(argc == 2);
	auto fmap = util::FileMap::Create(argv[1], util::FileMap::RDONLY);
	if (!fmap.has_value()) {
		return 1;
	}

	char *c = (char *)fmap->GetMem();
	for (size_t i = 0; i < fmap->GetSize(); ++i)
		std::cout << c[i];

	return 0;
}