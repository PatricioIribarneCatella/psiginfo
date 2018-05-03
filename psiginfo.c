#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define SIGNALS 20
#define BUFLEN 128

#define SIZE(X, Y) (sizeof(X)/sizeof(Y))

static char blk_sigs[BUFLEN];
static char ign_sigs[BUFLEN];
static char cgt_sigs[BUFLEN];

typedef struct {
	char* name;
	unsigned int num;
	unsigned long int hex_rep;
} Signal;

static Signal sig_hex[] = {
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
	{"SIGUSR2", 12, (1 << 11)},
	{"SIGPIPE", 13, (1 << 12)},
	{"SIGALRM", 14, (1 << 13)},
	{"SIGTERM", 15, (1 << 14)},
	{"SIGUSR1", 16, (1 << 15)},
	{"SIGUSR2", 17, (1 << 16)},
	{"SIGCONT", 18, (1 << 17)},
	{"SIGCONT", 19, (1 << 18)},
	{"SIGTSTP", 20, (1 << 19)},
	{"SIGTTIN", 21, (1 << 20)},
	{"SIGTTOU", 22, (1 << 21)},
	{"SIGSTOP", 23, (1 << 22)},
	{"SIGTSTP", 24, (1 << 23)},
	{"SIGCONT", 25, (1 << 24)},
	{"SIGTTIN", 26, (1 << 25)},
	{"SIGTTOU", 27, (1 << 26)},
	{"SIGWINCH", 28, (1 << 27)},
	{"SIGPROF", 29, (1 << 28)},
	{"SIGUSR1", 30, (1 << 29)},
	{"SIGUSR2", 31, (1 << 30)}
};

static void print_signal_status(const char* title, const char* sig_bitmask) {

	long unsigned int bit_mask = strtoul(sig_bitmask, NULL, 16);

	printf("%s (Mask: %lx)\n", title, bit_mask);
	
	for (int i = 0; i < SIZE(sig_hex, Signal); i++) {
		if (bit_mask & sig_hex[i].hex_rep)
			printf("\t Name: %s (Number: %u)\n",
				sig_hex[i].name,
				sig_hex[i].num);
	}

	printf("\n");
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
