
#include "./include/process.h"

int next_pid = 1;

PCB* create_process(void (*entry_point)(void)) {
    PCB* pcb = (PCB*)mm_malloc(sizeof(PCB));
    if (!pcb) return NULL;

    void* stack = mm_malloc(STACK_SIZE);
    if (!stack) {
        mm_free(pcb);
        return NULL;
    }

    // Empezamos desde el final de la pila
    uint64_t* sp = (uint64_t*)((uint8_t*)stack + STACK_SIZE);

    // Simulamos el contexto que espera iretq
    *(--sp) = 0x0;                        // SS (puede omitirse si no cambia de ring)
    *(--sp) = (uint64_t)(stack + STACK_SIZE); // RSP (user stack pointer)
    *(--sp) = 0x202;                      // RFLAGS
    *(--sp) = 0x8;                        // CS (kernel code segment)
    *(--sp) = (uint64_t)entry_point;      // RIP

    // Pusheamos GPRs en el orden inverso a tu pushState
    *(--sp) = 0; // r15
    *(--sp) = 0; // r14
    *(--sp) = 0; // r13
    *(--sp) = 0; // r12
    *(--sp) = 0; // r11
    *(--sp) = 0; // r10
    *(--sp) = 0; // r9
    *(--sp) = 0; // r8
    *(--sp) = 0; // rsi
    *(--sp) = 0; // rdi
    *(--sp) = 0; // rbp
    *(--sp) = 0; // rdx
    *(--sp) = 0; // rcx
    *(--sp) = 0; // rbx
    *(--sp) = 0; // rax

    pcb->stack_pointer = (uint64_t)sp;
    pcb->pid = next_pid++;

    add_to_ready_list(pcb);

    return pcb;
}