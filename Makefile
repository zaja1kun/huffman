CC := gcc
WFLAGS := -Wall -Wextra -Wshadow -Wstrict-overflow -Wpedantic
DBG_FLAGS := -O0 -g -save-temps
REL_FLAGS := -O3 -flto -march=native -mfpmath=sse
CFLAGS = -c -std=c11 $(REL_FLAGS) $(WFLAGS)
LDFLAGS =
CD := cd bin/temp;\

SOURCES=core.c haffman.c logging.c stdsafe.c pqueue.c btree.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=haff

.PHONY: all prepare_bin_dir clean

all: clean prepare_bin_dir $(SOURCES) $(EXECUTABLE)

prepare_bin_dir:
	mkdir -p bin/temp
	cp -r static/ bin/

$(EXECUTABLE): $(OBJECTS)
	$(CD) $(CC) $(LDFLAGS) $(OBJECTS) -o ../$@

.c.o:
	$(CD) $(CC) $(CFLAGS) ../../$< -o $@

clean:
	rm -rf *.o *.gch bin/$(EXECUTABLE) bin/temp
