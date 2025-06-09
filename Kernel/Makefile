include Makefile.inc

KERNEL_BIN=kernel.bin
KERNEL_ELF=kernel.elf
SOURCES_C = $(wildcard *.c interrupts/*.c drivers/*.c memory/*.c process/*.c syncro/*.c)
SOURCES_ASM = $(wildcard asm/*.asm)
OBJECTS_C = $(SOURCES_C:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)
LOADERSRC = loader.asm
LOADEROBJECT = $(LOADERSRC:.asm=.o)
STATICLIBS =

# Selección del administrador de memoria
ifeq ($(MEMORY_MANAGER),buddy)
    MEMORY_FLAGS = -DBUDDY_SYSTEM
else
    MEMORY_FLAGS = 
endif

all: $(KERNEL_BIN) $(KERNEL_ELF)

$(KERNEL_BIN): $(LOADEROBJECT) $(OBJECTS_ASM) $(OBJECTS_C) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^


$(KERNEL_ELF): $(LOADEROBJECT) $(OBJECTS_ASM) $(OBJECTS_C) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) $(MEMORY_FLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin memory/*.o interrupts/*.o *.elf drivers/*.o

.PHONY: all clean