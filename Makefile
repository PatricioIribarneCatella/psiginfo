CC  := gcc
CFLAGS := -Wall -std=c99 -g
EXEC := psiginfo
PID := 1

all:
	$(CC) $(CFLAGS) $(EXEC).c -o $(EXEC)

run: all
	./$(EXEC) $(PID)

clean:
	rm -f $(EXEC)

.PHONY: clean run
