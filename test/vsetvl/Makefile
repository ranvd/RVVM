
RISCV-PREFIX := riscv32-unknown-linux-gnu-
CC := gcc
CPY := objcopy
CFLAG := -march=rv32gv -mabi=ilp32 -nostdlib -Ttext=0x80000000

ASM := $(wildcard *.S)
ELF := $(ASM:.S=.elf)
BIN := $(ELF:.elf=.bin)

.PRECIOUS: %.elf
all: $(BIN)

%.bin: %.elf
	$(RISCV-PREFIX)$(CPY) -O binary $< $@

%.elf: %.S
	$(RISCV-PREFIX)$(CC) $(CFLAG) $< -o $@


.PHONY: clean
clean:
	-rm -f *.bin *.elf