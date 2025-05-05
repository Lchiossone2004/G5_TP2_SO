include ../Makefile.inc

MODULE_BIN=0000-sampleCodeModule.bin
MODULE_ELF=0000-sampleCodeModule.elf
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard c/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)

all: $(MODULE_BIN) $(MODULE_ELF)

$(MODULE_BIN): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE_BIN) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C)
$(MODULE_ELF): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_C)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../$(MODULE_ELF) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C)


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf asm/*.o c/*.o *.o

.PHONY: all clean print