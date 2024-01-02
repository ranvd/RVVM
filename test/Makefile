all: main.bin

main.bin: main.elf
	riscv-none-elf-objcopy -O binary main.elf main.bin

main.elf: start.S
	riscv-none-elf-gcc -nostdlib -Ttext=0x80000000 -march=rv32ima -mabi=ilp32 start.S main.c -o main.elf
	