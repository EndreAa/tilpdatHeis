EXECUTABLE = elevator
COMPILER = clang
CFLAGS = -Wall -g -std=gnu11 -fsanitize=address
LDFLAGS = -fsanitize=address

all: $(EXECUTABLE)

$(EXECUTABLE): build/door.o build/elevio.o build/lights.o build/main.o build/movement.o build/orders.o build/queue.o build/sensor.o build/start.o build/TSM.o
	$(COMPILER) build/door.o build/elevio.o build/lights.o build/main.o build/movement.o build/orders.o build/queue.o build/sensor.o build/start.o build/TSM.o -o $(EXECUTABLE) $(LDFLAGS)

build/door.o: source/door.c
	mkdir -p build
	$(COMPILER) -c $(CFLAGS) source/door.c -o build/door.o

build/elevio.o: source/driver/elevio.c
	mkdir -p build
	$(COMPILER) -c $(CFLAGS) source/driver/elevio.c -o build/elevio.o

build/lights.o: source/lights.c
	$(COMPILER) -c $(CFLAGS) source/lights.c -o build/lights.o

build/main.o: source/main.c
	$(COMPILER) -c $(CFLAGS) source/main.c -o build/main.o

build/movement.o: source/movement.c
	$(COMPILER) -c $(CFLAGS) source/movement.c -o build/movement.o

build/orders.o: source/orders.c
	$(COMPILER) -c $(CFLAGS) source/orders.c -o build/orders.o

build/queue.o: source/queue.c
	$(COMPILER) -c $(CFLAGS) source/queue.c -o build/queue.o

build/sensor.o: source/sensor.c
	$(COMPILER) -c $(CFLAGS) source/sensor.c -o build/sensor.o

build/start.o: source/start.c
	$(COMPILER) -c $(CFLAGS) source/start.c -o build/start.o

build/TSM.o: source/TSM.c
	$(COMPILER) -c $(CFLAGS) source/TSM.c -o build/TSM.o

clean:
	rm -f $(EXECUTABLE)
	rm -rf build

.PHONY: all clean



