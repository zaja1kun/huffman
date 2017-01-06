CC := gcc
WFLAGS := -Wall -Wextra -Wshadow -Wstrict-overflow -Wpedantic
DBG_FLAGS := -O0 -g -save-temps
REL_FLAGS := -O3 -flto -march=native -mfpmath=sse
CFLAGS = -c -std=c11 $(REL_FLAGS) $(WFLAGS)
LDFLAGS =
CD := cd bin/temp;\

SOURCES=core.c huffman.c logging.c stdsafe.c pqueue.c btree.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=huff

.PHONY: all prepare_bin_dir docs clean


all: clean prepare_bin_dir $(SOURCES) $(EXECUTABLE) docs

prepare_bin_dir:
	mkdir -p bin/temp

$(EXECUTABLE): $(OBJECTS)
	$(CD) $(CC) $(LDFLAGS) $(OBJECTS) -o ../$@

.c.o:
	$(CD) $(CC) $(CFLAGS) ../../$< -o $@

docs:
	doxygen doxyfile

clean:
	rm -rf *.o *.gch bin/$(EXECUTABLE) bin/temp
