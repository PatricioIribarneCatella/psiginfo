#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BUFLEN 128
#define PGSIZE 4096

// Rounding operations (efficient when n is a power of 2)
// Round down to the nearest multiple of n
#define ROUNDDOWN(a, n)                                         \
({                                                              \
        uint32_t __a = (uint32_t) (a);                          \
        (typeof(a)) (__a - __a % (n));                          \
})

// Round up to the nearest multiple of n
#define ROUNDUP(a, n)                                           \
({                                                              \
        uint32_t __n = (uint32_t) (n);                          \
        (typeof(a)) (ROUNDDOWN((uint32_t) (a) + __n - 1, __n)); \
})

static void print_signal_status(const char* title, const char* sig_bitmask) {

	printf("%s\n", title);
	printf("\t%s\n", sig_bitmask);
}

static void find_signal_info(const char* file) {

	static char blk_sigs[BUFLEN];
	static char ign_sigs[BUFLEN];
	static char cgt_sigs[BUFLEN];

	sscanf(file, "SigBlk: %s\nSigIgn: %s\nSigCgt: %s\n",
		blk_sigs, ign_sigs, cgt_sigs);

	print_signal_status("Blocked Signals", blk_sigs);
	print_signal_status("Ignored Signals", ign_sigs);
	print_signal_status("Caught Signals", cgt_sigs);
}

static void psiginfo(int pid) {

	int fd;
	char* file;
	struct stat sb;
	char buf[BUFLEN] = {0};

	snprintf(buf, sizeof buf, "/proc/%d/status", pid);

	if ((fd = open(buf, O_RDONLY, 0)) < 0) {
		perror("cannot open file ");
		_exit(EXIT_FAILURE);
	}

	if (fstat(fd, &sb) < 0) {
		perror("cannot stat file ");
		close(fd);	
		_exit(EXIT_FAILURE);
	}

	if ((file = mmap(NULL, ROUNDUP(sb.st_size, PGSIZE), PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		perror("cannot map file ");
		close(fd);
		_exit(EXIT_FAILURE);
	}

	close(fd);
	
	find_signal_info(file);

	if (munmap(file, sb.st_size) < 0) {
		perror("cannot unmap ");
		_exit(EXIT_FAILURE);
	}
}

int main(int arg, char const* argv[]) {

	// it must contains the pid
	assert(arg == 2);

	psiginfo(atoi(argv[1]));

	return 0;
}
