#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFLEN 128

static void psiginfo(int pid) {

	int fd;
	char buf[BUFLEN] = {0};

	snprintf(buf, sizeof buf, "/proc/%d/status", pid);

	if ((fd = open(buf, O_RDONLY, 0)) < 0) {
		perror("cannot open file ");
		_exit(EXIT_FAILURE);
	}

	/* Search in the file for "Sig* lines"*/
}

int main(int arg, char const* argv[]) {

	// it must contains the pid
	assert(arg == 2);

	psiginfo(atoi(argv[1]));

	return 0;
}
