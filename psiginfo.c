#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define BUFLEN 128
static char blk_sigs[BUFLEN];
static char ign_sigs[BUFLEN];
static char cgt_sigs[BUFLEN];

struct Signal {
	char* name;
	unsigned int num;
	unsigned long int hex_rep;
};

static struct Signal signals_hex_format[] = {
	{"SIGHUP", 1, (1 << 0)},
	{"SIGINT", 2, (1 << 1)},
	{"SIGQUIT", 3, (1 << 2)},
	{"SIGILL", 4, (1 << 3)},
	{"SIGTRAP", 5, (1 << 4)},
	{"SIGABRT", 6, (1 << 5)},
	{"SIGBUS", 7, (1 << 6)},
	{"SIGFPE", 8, (1 << 7)},
	{"SIGKILL", 9, (1 << 8)},
	{"SIGBUS", 10, (1 << 9)},
	{"SIGSEGV", 11, (1 << 10)},
};

static void print_signal_status(const char* title, const char* sig_bitmask) {

	printf("%s\n", title);
	printf("\t%lx\n", strtoul(sig_bitmask, NULL, 16));
}

static void find_signal_info(FILE* file) {

	char buf[BUFLEN] = {0};

	while (fgets(buf, sizeof buf, file) != NULL) {
		
		sscanf(buf, "SigBlk:\t%s\n", blk_sigs);
		sscanf(buf, "SigIgn:\t%s\n", ign_sigs);
		sscanf(buf, "SigCgt:\t%s\n", cgt_sigs);
	}

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
