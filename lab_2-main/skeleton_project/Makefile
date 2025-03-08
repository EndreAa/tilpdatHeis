EXECUTABLE  = elevator
COMPILER    = clang
CFLAGS      = -Wall -g -std=gnu11 -fsanitize=address
LDFLAGS     = -fsanitize=address


SOURCEDIR   = .
OBJECTDIR   = build


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


OBJECTS = $(addprefix $(OBJECTDIR)/, $(SOURCES:.c=.o))

all: $(EXECUTABLE)

rebuild: clean all

clean:
	rm -f $(EXECUTABLE)
	rm -rf $(OBJECTDIR)


$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $(OBJECTS) -o $@ $(LDFLAGS)


$(OBJECTDIR)/%.o: %.c
	@mkdir -p $(OBJECTDIR)
	$(COMPILER) -c $(CFLAGS) $< -o $@

.PHONY: all rebuild clean



