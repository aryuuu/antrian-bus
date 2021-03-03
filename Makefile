all: build run
run: build
	bin/antrian
build:
	gcc -lm src/main.c src/lib/simlib.c -o bin/antrian

clean:
	@echo "Cleaning up..."
