#include <assert.h>
#include "psiginfo.h"

int main(int argc, char const* argv[]) {

	if (argc < 2)
		printf("Usage: make run PID='pid1 pid2 ... pidn'\n"
			"\tPID has to be more than one\n");

	for (int i = 1; i < argc; i++)
		psiginfo(atoi(argv[i]));

	return 0;
}

