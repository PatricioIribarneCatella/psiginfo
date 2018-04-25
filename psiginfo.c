#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define BUFLEN 128

static void print_signal_status(const char* title, const char* sig_bitmask) {

	printf("%s\n", title);
	printf("\t%s\n", sig_bitmask);
}

static void find_signal_info(FILE* file) {

	static char blk_sigs[BUFLEN];
	static char ign_sigs[BUFLEN];
	static char cgt_sigs[BUFLEN];

	fscanf(file, "SigBlk: %s\nSigIgn: %s\nSigCgt: %s\n",
		blk_sigs, ign_sigs, cgt_sigs);

	print_signal_status("Blocked Signals", blk_sigs);
	print_signal_status("Ignored Signals", ign_sigs);
	print_signal_status("Caught Signals", cgt_sigs);
}

static void psiginfo(int pid) {

	FILE* f;
	char buf[BUFLEN] = {0};

	snprintf(buf, sizeof buf, "/proc/%d/status", pid);

	if (!(f = fopen(buf, "r"))) {
		perror("cannot open file ");
		_exit(EXIT_FAILURE);
	}
	
	find_signal_info(f);

	fclose(f);
}

int main(int arg, char const* argv[]) {

	// it must contains the pid
	assert(arg == 2);

	psiginfo(atoi(argv[1]));

	return 0;
}
