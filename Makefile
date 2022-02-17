PREFIX = /usr/local
BIN = $(PREFIX)/bin
EXE = "dvd"

all: build

build:
	$(CC) -o ./$(EXE) main.c

clean:
	rm -f ./$(EXE)

install:
	mkdir -p $(BIN)
	cp -f ./$(EXE) $(BIN)
	chmod 555 $(BIN)/$(EXE)

uninstall:
	rm -f $(BIN)/$(EXE)

.PHONY: all build clean install uninstall
