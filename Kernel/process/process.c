
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/lib.h"

#define STACK_SIZE 4096
#define MAX_PROCESSES 10

uint64_t createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[]) {
    void* stack_base = mm_malloc(STACK_SIZE);
    if (!stack_base) return -1;

    p_stack* stack_frame = (p_stack*)(stack_base + STACK_SIZE - sizeof(p_stack));
    memset(stack_frame, 0, sizeof(p_stack));

    stack_frame->rip = fn;
    stack_frame->rdi = (void*)(uint64_t)argc;
    stack_frame->rsi = argv;
    stack_frame->cs = (void*)0x08;
    stack_frame->rflags = (void*)0x202;
    stack_frame->ss = (void*)0x10;
    stack_frame->rsp = stack_frame;

    p_info* proc = mm_malloc(sizeof(p_info));
    //proc->pid = generate_pid();
    proc->pid = 3;
    proc->stack_base = stack_base;
    proc->stack_pointer = stack_frame;
    proc->argv = argv;
    proc->priority = 1;
    proc->state = READY;

    // for (int i = 0; i < MAX_PROCESSES; i++) {
    //     if (processes_list[i] == NULL) {
    //         processes_list[i] = proc;
    //         break;
    //     }
    // }

    //add_to_ready_list(proc);

    return proc->pid;
}