#include "psiginfo.h"

static char blk_sigs[BUFLEN];
static char ign_sigs[BUFLEN];
static char cgt_sigs[BUFLEN];

// signals hexdacimal representation
static Signal sig_hex[] = {
	{"SIGHUP", 1, (1ULL << 0)},
	{"SIGINT", 2, (1ULL << 1)},
	{"SIGQUIT", 3, (1ULL << 2)},
	{"SIGILL", 4, (1ULL << 3)},
	{"SIGTRAP", 5, (1ULL << 4)},
	{"SIGABRT", 6, (1ULL << 5)},
	{"SIGBUS", 7, (1ULL << 6)},
	{"SIGFPE", 8, (1ULL << 7)},
	{"SIGKILL", 9, (1ULL << 8)},
	{"SIGUSR1", 10, (1ULL << 9)},
	{"SIGSEGV", 11, (1ULL << 10)},
	{"SIGUSR2", 12, (1ULL << 11)},
	{"SIGPIPE", 13, (1ULL << 12)},
	{"SIGALRM", 14, (1ULL << 13)},
	{"SIGTERM", 15, (1ULL << 14)},
	{"SIGSTKFLT", 16, (1ULL << 15)},
	{"SIGCHLD", 17, (1ULL << 16)},
	{"SIGCONT", 18, (1ULL << 17)},
	{"SIGSTOP", 19, (1ULL << 18)},
	{"SIGTSTP", 20, (1ULL << 19)},
	{"SIGTTIN", 21, (1ULL << 20)},
	{"SIGTTOU", 22, (1ULL << 21)},
	{"SIGURG", 23, (1ULL << 22)},
	{"SIGXCPU", 24, (1ULL << 23)},
	{"SIGXFSZ", 25, (1ULL << 24)},
	{"SIGVTALRM", 26, (1ULL << 25)},
	{"SIGPROF", 27, (1ULL << 26)},
	{"SIGWINCH", 28, (1ULL << 27)},
	{"SIGIO", 29, (1ULL << 28)},
	{"SIGPWR", 30, (1ULL << 29)},
	{"SIGSYS", 31, (1ULL << 30)},
	{"SIGRTMIN", 34, (1ULL << 33)},
	{"SIGRTMIN+1", 35, (1ULL << 34)},
	{"SIGRTMIN+2", 36, (1ULL << 35)},
	{"SIGRTMIN+3", 37, (1ULL << 36)},
	{"SIGRTMIN+4", 38, (1ULL << 37)},
	{"SIGRTMIN+5", 39, (1ULL << 38)},
	{"SIGRTMIN+6", 40, (1ULL << 39)},
	{"SIGRTMIN+7", 41, (1ULL << 40)},
	{"SIGRTMIN+8", 42, (1ULL << 41)},
	{"SIGRTMIN+9", 43, (1ULL << 42)},
	{"SIGRTMIN+10", 44, (1ULL << 43)},
	{"SIGRTMIN+11", 45, (1ULL << 44)},
	{"SIGRTMIN+12", 46, (1ULL << 45)},
	{"SIGRTMIN+13", 47, (1ULL << 46)},
	{"SIGRTMIN+14", 48, (1ULL << 47)},
	{"SIGRTMIN+15", 49, (1ULL << 48)},
	{"SIGRTMAX-14", 50, (1ULL << 49)},
	{"SIGRTMAX-13", 51, (1ULL << 50)},
	{"SIGRTMAX-12", 52, (1ULL << 51)},
	{"SIGRTMAX-11", 53, (1ULL << 52)},
	{"SIGRTMAX-10", 54, (1ULL << 53)},
	{"SIGRTMAX-9", 55, (1ULL << 54)},
	{"SIGRTMAX-8", 56, (1ULL << 55)},
	{"SIGRTMAX-7", 57, (1ULL << 56)},
	{"SIGRTMAX-6", 58, (1ULL << 57)},
	{"SIGRTMAX-5", 59, (1ULL << 58)},
	{"SIGRTMAX-4", 60, (1ULL << 59)},
	{"SIGRTMAX-3", 61, (1ULL << 60)},
	{"SIGRTMAX-2", 62, (1ULL << 61)},
	{"SIGRTMAX-1", 63, (1ULL << 62)},
	{"SIGRTMAX", 64, (1ULL << 63)}
};

// prints signals information in a
// human friendly way
static void print_signal_status(const char* title, const char* sig_bitmask) {

	long unsigned int bit_mask = strtoul(sig_bitmask, NULL, 16);

	printf("%s (Mask: %lx)\n", title, bit_mask);
	
	for (int i = 0; i < LENGHT(sig_hex); i++) {
		if (bit_mask & sig_hex[i].hex_rep)
			printf("\t Name: %s (Number: %u)\n",
				sig_hex[i].name,
				sig_hex[i].num);
	}

	printf("\n");
}

// finds signal status information
// within the file
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

// reads and searches for signals status
// information in the /proc/[pid]/status file
static void print_signal_info(int pid) {
	
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

// prints command line
// representation of the process
void print_cmdline_info(int pid) {

	FILE* f;
	int i = 0, j = 1, k = 2;
	char buf[BUFLEN] = {0};
	char* r;

	snprintf(buf, sizeof buf, "/proc/%d/cmdline", pid);
	
	if (!(f = fopen(buf, "r"))) {
		perror("cannot open file ");
		_exit(EXIT_FAILURE);
	}
	
	memset(buf, 0, sizeof buf);
	r = fgets(buf, sizeof buf, f);

	fclose(f);

	if (!r)
		return;

	while (buf[j] != '\0' || buf[k] != '\0') {
		if (buf[i] == '\0')
			buf[i] = ' ';
		i++; j++; k++;
	}

	printf("cmdline name: %s\n\n", buf);
}

// prints what /proc/[pid]/comm
// holds in (the command name)
void print_comm_info(int pid) {

	FILE* f;
	char buf[BUFLEN] = {0};

	snprintf(buf, sizeof buf, "/proc/%d/comm", pid);
	
	if (!(f = fopen(buf, "r"))) {
		perror("cannot open file ");
		_exit(EXIT_FAILURE);
	}
	
	memset(buf, 0, sizeof buf);
	fgets(buf, sizeof buf, f);

	fclose(f);

	printf("name: %s", buf);
}

// prints information about the
// process´ command corresponding to 'pid'
static void print_cmd_info(int pid) {

	printf("pid: %d\n", pid);

	print_comm_info(pid);
	print_cmdline_info(pid);
}

// prints information about
// process´ signals status
void psiginfo(int pid) {

	print_cmd_info(pid);
	print_signal_info(pid);
}

