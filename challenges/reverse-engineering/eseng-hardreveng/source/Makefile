CC = gcc
binaries = shift_satellite
build_path = ./build

main: $(binaries)

$(binaries): %:%.c
	$(CC) -o $(build_path)/$@ $<
