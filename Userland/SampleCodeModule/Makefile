include ../Makefile.inc

MODULE_BIN=0000-sampleCodeModule.bin
MODULE_ELF=0000-sampleCodeModule.elf
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_C=$(wildcard libs-c/*.c)
TEST_C=$(wildcard test/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)
OBJECTS_TEST=$(TEST_C:.c=.o)

all: $(MODULE_BIN) $(MODULE_ELF)

$(MODULE_BIN): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_C) $(OBJECTS_TEST)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE_BIN) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_TEST)
$(MODULE_ELF): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS) $(OBJECTS_C) $(OBJECTS_TEST)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../$(MODULE_ELF) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_C) $(OBJECTS_TEST)


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf asm/*.o libs-c/*.o *.o test/*.o

.PHONY: all clean print