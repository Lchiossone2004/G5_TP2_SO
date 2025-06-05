#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/videoDriver.h"
#include "../include/lib.h"
#include "../include/naiveConsole.h"

#define MAX_PROCESSES 10
#define MAX_PRIORITY 10
#define BLANCO  0xFFFFFF
#define COL_WIDTH 18


p_info* processes_list[MAX_PROCESSES];
int n_processes = 0;
int shellCorriendo = 0;

ReadyNode* ready_list = NULL;
ReadyNode* current_node = NULL;
p_info* current_process = NULL;

void* scheduler(void* current_sp) {
    if (!ready_list)
        return current_sp;
   

    if (current_process && current_process->state == RUNNING) {
        current_process->stack_pointer = current_sp;
        if (--current_node->counter > 0) {
            return current_sp;
        }
        current_process->state = READY;
    }
    ReadyNode* start = current_node;
    do {
        current_node = current_node->next;
        if (current_node->process_info->state == READY) {
            current_process = current_node->process_info;
            current_process->state = RUNNING;
            current_node->counter = current_process->priority;  
            return current_process->stack_pointer;
        }
    } while (current_node != start);
    if (current_process) {
        current_process->state = RUNNING;
        current_node->counter = current_process->priority;
        return current_process->stack_pointer;
    }
    return current_sp; 
}


void add_to_ready_list(p_info* process) {
    ReadyNode* node = mm_malloc(sizeof(ReadyNode));
    node->process_info = process;
    node->counter = process->priority;


    if (!ready_list) {
        ready_list = node;
        node->next = node;
        current_node = node;
    } else {
        ReadyNode* tail = ready_list;
        while (tail->next != ready_list)
            tail = tail->next;
        tail->next = node;
        node->next = ready_list;
    }
}

void remove_from_ready_list(p_info* process) {
    if (!ready_list) return;

    ReadyNode *curr = ready_list, *prev = NULL;
    do {
        if (curr->process_info == process) {
            if (curr == ready_list) ready_list = curr->next;

            if (prev) prev->next = curr->next;

            ReadyNode* to_free = curr;
            curr = curr->next;

            if (to_free == to_free->next) {
                mm_free(to_free);
                ready_list = NULL;
                current_node = NULL;
                return;
            }

            mm_free(to_free);
            continue;
        }

        prev = curr;
        curr = curr->next;
    } while (curr != ready_list);
}

int block_process(uint16_t pid) {
    int idx = foundprocess(pid);
    if (idx != -1 && processes_list[idx]->state == RUNNING || processes_list[idx]->state == READY) {
        processes_list[idx]->state = BLOCKED;
        callScheduler();
        return 1;
    }
    return 0;
}

int unblock_process(uint16_t pid) {
    int idx = foundprocess(pid);
    if (idx != -1 && processes_list[idx]->state == BLOCKED) {
        processes_list[idx]->state = READY;
        add_to_ready_list(processes_list[idx]);
        scheduler(get_current_process());
        return 1;
    }
    return 0;
}

void add_to_process_list(p_info* process) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] == NULL) {
            processes_list[i] = process;
            break;
        }
    }
    n_processes++;
}

p_info* find_process_by_stack(void* sp) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] && processes_list[i]->stack_pointer == sp)
            return processes_list[i];
    }
    return NULL;
}

p_info* get_current_process() {
    return current_process;
}

int kill_process(uint64_t pid) {
    int idx = foundprocess(pid);
    if (idx == -1)
        return 1;

    p_info* p = processes_list[idx];

    if (p == current_process) {
        p->state = TERMINATED;
        remove_from_ready_list(p);
        
    } else {
        remove_from_processes_list(p);
    }

    return 0;
}


int modify_priority(uint16_t pid, int newPriority) {
    int idx = foundprocess(pid);
    if (idx == -1)
        return 0;

    p_info* process = processes_list[idx];
    process->priority = newPriority;
    ReadyNode* node = ready_list;
    if (node) {
        do {
            if (node->process_info == process) {
                node->counter = newPriority;
                break;
            }
            node = node->next;
        } while (node != ready_list);
    }

    return 1;
}


int foundprocess(uint16_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] && processes_list[i]->pid == pid)
            return i;
    }
    return -1;
}

void printFixed(const char* str) {
    int len = strSize((char*)str);
    imprimirVideo((char*)str, len, BLANCO);

    for (int i = len; i < COL_WIDTH; i++) {
        imprimirVideo(" ", 1, BLANCO);
    }
}

void get_processes() {
    char buffer[32];

    printFixed("PID");
    printFixed("Name");
    printFixed("Priority");
    printFixed("Stack Base");
    printFixed("Stack Pointer");
    printFixed("State");
    printFixed("Foreground");
    nlVideo();

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (!processes_list[i]) continue;

        uintToBase(processes_list[i]->pid, buffer, 10);
        printFixed(buffer);

        printFixed(processes_list[i]->name);

        printFixed(processes_list[i]->priorityName);

        uintToBase((uint64_t)processes_list[i]->stack_base, buffer, 16);
        printFixed(buffer);

        uintToBase((uint64_t)processes_list[i]->stack_pointer, buffer, 16);
        printFixed(buffer);

        const char *estado;
        switch (processes_list[i]->state) {
            case READY: estado = "READY"; break;
            case RUNNING: estado = "RUNNING"; break;
            case BLOCKED: estado = "BLOCKED"; break;
            case TERMINATED: estado = "TERMINATED"; break;
            default: estado = "UNKNOWN"; break;
        }
        printFixed(estado);

        printFixed(processes_list[i]->is_foreground == 1 ? "YES" : "NO");
        nlVideo();
    }
}

uint16_t quitCPU() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] && processes_list[i]->state == RUNNING) {
            processes_list[i]->state = READY;
            add_to_ready_list(processes_list[i]);
            scheduler(processes_list[i]->stack_pointer);
            return processes_list[i]->pid;
        }
    }
    return (uint16_t)-1;
}

int remove_from_processes_list(p_info* process) {
    int idx = foundprocess(process->pid);
    if (idx < 0 || idx >= MAX_PROCESSES || processes_list[idx] == NULL) {
        return -1; 
    }
    process->state = TERMINATED;
    remove_from_ready_list(process);
    mm_free(process->stack_top); //okey este es el problema
    mm_free(process->name);
    mm_free(process->priorityName);
    mm_free(process);
    processes_list[idx] = NULL;
    n_processes--;

    return 0; 
}

p_info* get_foreground_process(){
    for(int i = 0; i <MAX_PROCESSES; i++){
        if(processes_list[i]->is_foreground){
            return processes_list[i];
        }
    }   
    return -1;
}

