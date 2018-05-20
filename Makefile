CFLAGS := -std=c99 -g -Wall
CC := gcc
EXEC = main
BIN = $(filter-out $(EXEC).c, $(wildcard *.c))
BINFILES = $(BIN:.c=.o)
OBJDIR = build
PID =

$(OBJDIR)/%.o: %.c %.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

OBJEXEC = $(addprefix $(OBJDIR)/, $(EXEC))

all: $(OBJEXEC)

$(OBJEXEC): $(addprefix $(OBJDIR)/, $(BINFILES)) $(EXEC).c
	$(CC) $(CFLAGS) $^ -o $@

run: all
	./$(OBJDIR)/$(EXEC) $(PID)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(OBJDIR)/$(EXEC)

clean:
	rm -rf $(OBJDIR)/ *.o *.asm

.PHONY: clean run
