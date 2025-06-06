#include "../include/process.h"
#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/lib.h"

#define STACK_SIZE 4096
#define MAX_PROCESSES 20

extern void callScheduler();
static int pirority[] = {8, 4,2,1}; //8 =critica, 4=alta,2=normal,1=baja
static char* pirorityName[] = {"Critic", "High", "Normal", "Low"};
static int pids[MAX_PROCESSES] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //ESTO ME PARECE QUE ESTA DEMAS CON {0} ALCANZA 


int createProcess(void (*fn)(uint8_t, char **), uint8_t argc, char* argv[], char* name, int prio, int is_foreground) {

    void* stack_top = mm_malloc(STACK_SIZE) ;
    void* stack_base = stack_top + STACK_SIZE;
    if (!stack_top) return -1;

    p_info* new_process = mm_malloc(sizeof(p_info));
    if (!new_process) return -1;
    p_stack* new_stack = stack_base - sizeof(p_stack);

    copy_context(new_process, name, stack_base, new_stack, stack_top,prio, is_foreground);

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
    pids[current->pid-1] = 0;
    callScheduler();
}

int get_pid() {
    p_info* current = get_current_process();
    return current->pid;
}

int fork() {
    p_info* current = get_current_process();
    if (current->children_length >= MAX_CHILDREN)
        return -1;

    void* stack_base = mm_malloc(STACK_SIZE);
    if (!stack_base) return -1;

    void* stack_top = stack_base + STACK_SIZE;
    p_stack* new_stack = stack_top - sizeof(p_stack);

    p_info* new_process = mm_malloc(sizeof(p_info));
    if (!new_process) {
        mm_free(stack_base);
        return -1;
    }

    copy_context(new_process, current->name, stack_base, new_stack, stack_top, current->priority, current->is_foreground);

    new_process->argc = current->argc;
    if (current->argc > 0 && current->argv) {
        new_process->argv = mm_malloc((current->argc + 1) * sizeof(char*));
        for (int i = 0; i < current->argc; i++) {
            uint64_t len = strSize(current->argv[i]) + 1;
            new_process->argv[i] = mm_malloc(len * sizeof(char));
            memcpy(new_process->argv[i], current->argv[i], len);
        }
        new_process->argv[current->argc] = NULL;
    } else {
        new_process->argv = NULL;
    }


    current->children[current->children_length] = new_process->pid;
    current->children_length++;

    add_to_process_list(new_process);
    add_to_ready_list(new_process);

    return new_process->pid;
}


void copy_context(p_info* new_process, char *name, void * stack_base, void * stack_pointer,void * stack_top ,int prio, int is_foreground) {

    int len = strSize(pirorityName[prio]);
    assignPid(new_process);
    new_process->name = mm_malloc(strSize(name) + 1);
    memcpy(new_process->name, name, strSize(name));
    new_process->stack_base = stack_base;
    new_process->stack_pointer = stack_pointer;
    new_process->stack_top = stack_top;
    new_process->state = READY;
    new_process->priority = pirority[prio];
    new_process->priorityName = mm_malloc(len * sizeof(char));
    memcpy(new_process->priorityName,pirorityName[prio],len);
    initialize_zero(new_process->children, MAX_CHILDREN);
    new_process->children_length = 0;
    assignForeground(new_process, is_foreground);
    new_process->stdin = 0;
    new_process->stdout = 1;
    for(int i = 0; i <MAX_BUFF; i++){
        new_process->buffers[i] = 0;
    }
}

int wait_pid(int pid) {
    p_info* current = get_current_process();
    for (int i = 0; i < current->children_length; i++) {
        if (current->children[i] == pid) {
            while (1) {
                if (foundprocess(pid) == -1) {
                    current->children[i] = 0;
                    return pid;
                }
            }
            break;
        }
    }
    return -1;
}

int wait() {
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

void assignForeground(p_info* new_process, int is_foreground){
    new_process->is_foreground = is_foreground;
    if(new_process->is_foreground){
        p_info* foreground_proc = get_foreground_process();
        if(foreground_proc->pid != new_process->pid){
            foreground_proc->is_foreground = 0;
        }
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

int Dup(int pid,int newFd, int oldFd){
    if(oldFd>MAX_BUFF*2 || oldFd < 0){
        return;
    }
    p_info * proc = get_process_by_pid(pid);
    if(oldFd == 0){
        proc->stdin = newFd;
    }
    else if(oldFd == 1){
        proc->stdout = newFd;
    }
    else{
        for(int i = 0; i < MAX_BUFF * 2; i++){
            if(proc->buffers[i] == oldFd){
                proc->buffers[i] = newFd;
                break;
            }
        }
    }
}