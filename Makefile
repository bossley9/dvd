PREFIX = /usr/local
BIN = $(PREFIX)/bin
EXE = "dvd"

CFLAGS += -Wall -Werror

all: build

build:
	$(CC) $(CFLAGS) -o ./$(EXE) main.c -lncurses

clean:
	rm -f ./$(EXE)

install:
	mkdir -p $(BIN)
	cp -f ./$(EXE) $(BIN)
	chmod 555 $(BIN)/$(EXE)

uninstall:
	rm -f $(BIN)/$(EXE)

.PHONY: all build clean install uninstall
