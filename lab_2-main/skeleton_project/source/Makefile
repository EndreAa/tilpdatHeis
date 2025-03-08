EXECUTABLE  = elevator
COMPILER    = clang
CFLAGS      = -Wall -g -std=gnu11 -fsanitize=address
LDFLAGS     = -fsanitize=address

SOURCEDIR = source
OBJECTDIR = build

# Explicitly list all source files
SOURCES = door.c \
          lights.c \
          main.c \
          movement.c \
          orders.c \
          queue_test.c \
          queue.c \
          sensor.c \
          start.c \
          TSM.c

# Generate object file names from source files
OBJECTS = $(addprefix $(OBJECTDIR)/, $(SOURCES:.c=.o))

all: $(EXECUTABLE)

rebuild: clean all

clean:
	rm -f $(EXECUTABLE)
	rm -rf $(OBJECTDIR)

## executable depends on object files: link them
$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $^ -o $@ $(LDFLAGS)

## object files depend on source files: compile them
$(OBJECTDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(COMPILER) -o $@ -c $(CFLAGS) $

.PHONY: all rebuild clean



