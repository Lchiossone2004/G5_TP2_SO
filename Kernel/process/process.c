
#include "../include/process.h"
#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/lib.h"

#define STACK_SIZE 4096
#define MAX_PROCESSES 10

// extern void* setup_process_stack(void (*fn)(uint8_t, char**), uint8_t argc, char** argv, void* stack_top);

 static uint64_t next_pid = 1;
uint64_t createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[], char* name, int priority, int is_foreground) {

    void* stack_top = mm_malloc(STACK_SIZE) ;
    void* stack_base = stack_top + STACK_SIZE;
    if (!stack_top) return -1;

    p_info* new_process = mm_malloc(sizeof(p_info));
    if (!new_process) return -1;

    p_stack* new_stack = stack_base - sizeof(p_stack);

    copy_context(new_process, name, stack_base, new_stack, priority, is_foreground);    
    new_stack->rbp = stack_base;
    new_stack->rsp = stack_base;
    new_stack->cs = (void*)0x8;
    new_stack->rflags = (void*)0x202;
    new_stack->ss = 0x0;
    new_stack->rip = entry_point_wrapper;
    new_stack->rdi = fn;
    new_stack->rsi = (void*)argc;
    new_stack->rdx = argv;

    add_to_process_list(new_process);
    add_to_ready_list(new_process);

    return new_process->pid;
}



void entry_point_wrapper(void (*fn)(uint8_t, char**), uint8_t argc, char** argv) {
    fn(argc, argv);
    exit_process();
}

void exit_process() {
    p_info* current = get_current_process();  

    current->state = TERMINATED;
    remove_from_ready_list(current);  // o remove_from_process_list
    // Liberar recursos si corresponde
    mm_free(current->stack_base);
    mm_free(current);  // si usás malloc para los procesos
    while (1);  // por seguridad
}

uint16_t get_pid() {
    p_info* current = get_current_process();
    return current->pid;
}

uint16_t fork() {
    p_info* current = get_current_process();
    p_info* new_process = mm_malloc(sizeof(p_info));
    if (!new_process || current->children_length >= MAX_CHILDREN) return -1;

    copy_context(new_process, current->name, current->stack_base, current->stack_pointer, current->priority, current->is_foreground);
    current->children[current->children_length] = new_process->pid;
    current->children_length++;
    add_to_process_list(new_process);
    add_to_ready_list(new_process);

    return new_process->pid;
}

void copy_context(p_info* new_process, char *name, void * stack_base, void * stack_pointer, int priority, int is_foreground) {
    new_process->pid = next_pid++;
    new_process->name = mm_malloc(strSize(name) + 1);
    memcpy(new_process->name, name, strSize(name));
    new_process->stack_base = stack_base;
    new_process->stack_pointer = stack_pointer;
    new_process->state = READY;
    new_process->priority = priority;
    new_process->is_foreground = is_foreground;
    initialize_zero(new_process->children, MAX_CHILDREN);
    new_process->children_length = 0;
}

uint16_t wait_pid(uint16_t pid) {
    p_info* current = get_current_process();
    for (int i = 0; i < current->children_length; i++) {
        if (current->children[i] == pid) {
            while (1) {
                // Esperar a que el proceso hijo termine
                if (foundprocess(pid) == -1) {
                    current->children[i] = 0;  // Eliminar el PID de la lista de hijos
                    return pid;
                }
            }
            break;
        }
    }
    return -1;  // Proceso hijo no encontrado
}
uint16_t wait() {
    p_info* current = get_current_process();
    for(int i = 0; i < current->children_length; i++) {
        if(current->children[i] != 0) {
            wait_pid(current->children[i]);
            
        }
    }
    return 0;
}

void initialize_zero(uint16_t array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}
