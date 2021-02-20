all: build run
run: build
	./antrian
build:
	gcc -lm main.c simlib.c -o antrian

clean:
	@echo "Cleaning up..."