CC = gcc
binaries = shift_satellite simulate_satellites
shared = connection_helper
build_path = ./build

main: clean $(shared) $(binaries)

all: $(shared) $(binaries)

$(binaries): %:%.c
	$(CC) -o $(build_path)/$@ $< -L$(build_path) -l$(shared)

$(shared): %:%.c
	$(CC) -o $(build_path)/$@.o $< -fpic -c
	$(CC) -o $(build_path)/lib$@.so $< --shared
	rm $(build_path)/*.o

clean:
	rm -rf $(build_path)/*

shared: $(shared)

binaries: $(binaries)
