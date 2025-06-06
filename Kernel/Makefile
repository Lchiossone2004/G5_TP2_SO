include Makefile.inc

KERNEL_BIN=kernel.bin
KERNEL_ELF=kernel.elf
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_INTERRUPTS = $(wildcard interrupts/*.c)
SOURCES_DRIVERS = $(wildcard drivers/*.c)
SOURCES_MEMORY = $(wildcard memory/*.c)
SOURCES_PROCESS = $(wildcard process/*.c)
SOURCES_SYNCRO = $(wildcard syncro/*.c)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_INTERRUPTS=$(SOURCES_INTERRUPTS:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_MEMORY=$(SOURCES_MEMORY:.c=.o)
OBJECTS_PROCESS=$(SOURCES_PROCESS:.c=.o)
OBJECTS_SYNCRO=$(SOURCES_SYNCRO:.c=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

# Selección del administrador de memoria
ifeq ($(MEMORY_MANAGER),buddy)
    MEMORY_FLAGS = -DBUDDY_SYSTEM
else
    MEMORY_FLAGS = 
endif

all: $(KERNEL_BIN) $(KERNEL_ELF)

$(KERNEL_BIN): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEMORY) $(OBJECTS_PROCESS) $(OBJECTS_SYNCRO)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL_BIN) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEMORY) $(OBJECTS_PROCESS) $(OBJECTS_SYNCRO) $(STATICLIBS)

$(KERNEL_ELF): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEMORY) $(OBJECTS_PROCESS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o $(KERNEL_ELF) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_INTERRUPTS) $(OBJECTS_DRIVERS) $(OBJECTS_MEMORY) $(OBJECTS_PROCESS) $(OBJECTS_SYNCRO) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(MEMORY_FLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin memory/*.o interrupts/*.o *.elf drivers/*.o

.PHONY: all clean