CC = gcc
binaries = shift_satellite simulate_satellites
shared = connection_helper
build_path = ./build

main: $(shared) $(binaries)

$(binaries): %:%.c
	$(CC) -o $(build_path)/$@ $< -L$(build_path) -l$(shared)

$(shared): %:%.c
	$(CC) -o $(build_path)/$@.o $< -fpic -c
	$(CC) -o $(build_path)/lib$@.so $< --shared
	rm $(build_path)/*.o
