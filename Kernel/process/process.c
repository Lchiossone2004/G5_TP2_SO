
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/lib.h"

#define STACK_SIZE 4096
#define MAX_PROCESSES 10

extern void* setup_process_stack(void (*fn)(uint8_t, char**), uint8_t argc, char** argv, void* stack_top);


uint64_t createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[]) {
    void* stack_base = mm_malloc(STACK_SIZE);
    if (!stack_base) return -1;

    void* new_stack_top = stack_base + STACK_SIZE;
    void* new_rsp = setup_process_stack(fn, argc, argv, new_stack_top);

    p_info* proc = mm_malloc(sizeof(p_info));
    proc->pid = 3;
    proc->stack_base = stack_base;
    proc->stack_pointer = new_rsp;  // este es el stack que usará el proceso
    proc->argv = argv;
    proc->priority = 10;
    proc->state = READY;

    add_to_process_list(proc);
    add_to_ready_list(proc);

    return proc->pid;
}

void exit_process();  // Deberías implementar esto

void entry_point_wrapper(void (*fn)(uint8_t, char**), uint8_t argc, char** argv) {
    fn(argc, argv);
    exit_process();
}

void exit_process() {
    p_info* current = get_current_process();  // asumimos que esto existe

    current->state = TERMINATED;

    // Liberar recursos si corresponde
    mm_free(current->stack_base);
    mm_free(current);  // si usás malloc para los procesos

    remove_from_ready_list(current);  // o remove_from_process_list
    while (1);  // por seguridad
}