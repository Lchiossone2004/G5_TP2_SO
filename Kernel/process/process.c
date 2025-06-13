#include "../include/process.h"
#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/lib.h"
#include "../include/videoDriver.h"

#define STACK_SIZE 4096


extern void callScheduler();
static int pids[MAX_PROCESSES] = {0}; 


int createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[], char* name, int prio, int is_foreground) {
    void* stack_top = mm_malloc(STACK_SIZE) ;
    void* stack_base = stack_top + STACK_SIZE;
    if (!stack_top) {
        return -1;
    }

    p_info* new_process = mm_malloc(sizeof(p_info));
    if (!new_process) {
        return -1;
    }
    p_stack* new_stack = stack_base - sizeof(p_stack);
    copy_context(new_process, name, stack_base, new_stack, stack_top,prio, is_foreground);
    p_info* parent = get_current_process();
    if (parent) {
      new_process->parent_pid = parent->pid;
      if (parent->children_length < MAX_CHILDREN) {
        parent->children[parent->children_length++] = new_process->pid;
      }
    } else {

      new_process->parent_pid = 0;
    }

    new_stack->rbp = stack_base;
    new_stack->rsp = stack_base;
    new_stack->cs = (void*)0x8;
    new_stack->rflags = (void*)0x202;
    new_stack->ss = 0x0;
    new_stack->rip = entry_point_wrapper;
    new_stack->rdi = fn;
    load_args(new_stack, new_process, argc, argv);
    add_to_process_list(new_process);
    add_to_ready_list(new_process);

    for (int i = 0; i < MAX_BUFF*2; i++) {
            new_process->fd_table[i] = -1;
        }
    if (parent) {
        for (int i = 0; i < MAX_BUFF*2; i++) {
            pipe_accses(new_process->pid,parent->fd_table[i]);
        }
        parent->waiting_on_child++;
    }
    new_process->stdin = STDIN;
    new_process->stdout = STDOUT;
    return new_process->pid;
}

void load_args(p_stack *new_stack, p_info * new_process, uint8_t argc, char* argv[]){
    char ** argv_l = mm_malloc((argc+1) * sizeof(char *));
    for(uint8_t i=0; i < argc; i++){
        uint64_t length = strSize(argv[i]) + 1;
        argv_l[i] = mm_malloc(length * sizeof(char));
        memcpy(argv_l[i], argv[i], length);
    }
    argv_l[argc] = NULL;
    new_process->argc = argc;
    new_process->argv = argv_l;
    new_stack->rsi = (void *)(uint64_t)argc;
    new_stack->rdx = argv_l;
}

void entry_point_wrapper(void (*fn)(uint8_t, char**), uint8_t argc, char** argv) {
    fn(argc, argv);
    exit_process();
}

void exit_process() {
    p_info* current = get_current_process();
    remove_from_processes_list(current);
    for(int i = 0; i<MAX_BUFF*2;i++){
        pipe_close(current->fd_table[i]);
    }
    pids[current->pid-1] = 0;
    callScheduler();
}

int get_pid() {
    p_info* current = get_current_process();
    return current->pid;
}



void copy_context(p_info* new_process, char *name, void *stack_base, void *stack_pointer, void *stack_top, int prio, int is_foreground) {
    assignPid(new_process);

    size_t nameLen = strSize(name) + 1;
    new_process->name = mm_malloc(nameLen);
    memcpy(new_process->name, name, nameLen);
    new_process->stack_base = stack_base;
    new_process->stack_pointer = stack_pointer;
    new_process->stack_top = stack_top;

    new_process->state    = READY;
    new_process->priority = priority[prio];
    size_t prioNameLen = strSize(priorityName[prio]) + 1;
    new_process->priorityName = mm_malloc(prioNameLen);
    memcpy(new_process->priorityName, priorityName[prio], prioNameLen);
    initialize_zero(new_process->children, MAX_CHILDREN);
    new_process->children_length = 0;
    assignForeground(new_process, is_foreground);
}

// int wait_pid(int pid) {        
//     p_info *current = get_current_process();

//     int toRet = 0;
//     if (foundprocess(pid) != -1 && current->waiting_on_child > 0) {
//         toRet ++;              
//     }
//     for (int i = 0; i < current->children_length; i++) {
//         if (current->children[i] == pid) {
//             current->children[i] = 0;
//             break;
//         }
//    }
//    return toRet;
// }


int wait() {
    p_info *current = get_current_process();
   if(current->waiting_on_child > 0){
    block_process(current->pid);
    callScheduler();
    return 0;
   }
   else{
    return -1;
   }
}

void initialize_zero(uint16_t array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

void assignForeground(p_info* new_p, int is_fg) {
    new_p->is_foreground = is_fg;
    if (is_fg) {
        p_info *current_fg = get_foreground_process();
        current_fg->is_foreground = 0;
        imprimirPrograma(new_p->name,strSize(new_p->name));
    }
}


void assignPid(p_info* new_process){
    new_process->pid = 0;
    for(int i = 0; i<MAX_PROCESSES; i++){
        if(pids[i] == 0){
            new_process->pid = (int32_t)(i + 1);
            pids[i] = 1;
            return;
        }
    }
}

void freePid(int pid){
    if(pid>0 && pid< MAX_PROCESSES){
        pids[pid-1] = 0;
    }
}
