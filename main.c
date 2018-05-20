#include <assert.h>
#include "psiginfo.h"

int main(int argc, char const* argv[]) {

	assert(argc >= 2);

	for (int i = 1; i < argc; i++)
		psiginfo(atoi(argv[i]));

	return 0;
}

