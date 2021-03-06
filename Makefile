PROGRAM_NAME = ssbf
VERSION = 0.1
CC = clang
CFLAGS = -Wall -pedantic -DVERSION=$(VERSION) -std=gnu11
DEBUG = -g
OPT = -O0
ARCHIVE = $(PROGRAM_NAME)_$(VERSION)
LDFLAGS = -lz
SDIR = .

all: clean build

bloom.o: $(SDIR)/bloom.h $(SDIR)/bloom.c
	$(CC) $(CFLAGS) $(DEBUG) -c $(SDIR)/$*.c
filter.o: $(SDIR)/filter.c $(SDIR)/ssbf.h bloom.o
	$(CC) $(CFLAGS) $(DEBUG) -c $(SDIR)/$*.c
index.o: $(SDIR)/index.c $(SDIR)/ssbf.h bloom.o
	$(CC) $(CFLAGS) $(DEBUG) -c $(SDIR)/$*.c
ssbf.o: $(SDIR)/ssbf.c $(SDIR)/ssbf.h filter.o bloom.o
	$(CC) $(CFLAGS) $(DEBUG) -c $(SDIR)/$*.c
bloom: bloom.o $(SDIR)/ssbf.h
	$(CC) $(CFLAGS) $(LDFLAGS) $? -o bloom

clean:
	rm -rf *.o ./$(PROGRAM_NAME) *.dSYM

build: bloom.o index.o filter.o ssbf.o
	$(CC) $(CFLAGS) $(LDFLAGS) $? -o $(PROGRAM_NAME)
