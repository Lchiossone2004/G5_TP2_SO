#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#include "../include/videoDriver.h"
#include "../include/lib.h"
#include "../include/naiveConsole.h"
#include "../include/process.h"

p_info* processes_list[MAX_PROCESSES];
int n_processes = 0;
int shellCorriendo = 0;

ReadyNode* ready_list = NULL;
ReadyNode* current_node = NULL;
p_info* current_process = NULL;

void* scheduler(void* current_sp) {
    if (!ready_list)
        return current_sp;
    if(current_process->state == BLOCKED){
        current_process->stack_pointer = current_sp;
    }

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

    ReadyNode *curr = ready_list;
    ReadyNode *prev = NULL;

    do {
        if (curr->process_info == process) {
            if (curr->next == curr) {
                mm_free(curr);
                ready_list = NULL;
                current_node = NULL;
                return;
            }
            if (curr == ready_list) {
                ready_list = curr->next;
            }
            if (prev) {
                prev->next = curr->next;
            } else {
                ReadyNode *last = ready_list;
                while (last->next != curr) {
                    last = last->next;
                }
                last->next = curr->next;
            }
            mm_free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    } while (curr != ready_list);
}

int block_process(int pid) {
    int idx = foundprocess(pid);
    if (idx != -1 && (processes_list[idx]->state == RUNNING || processes_list[idx]->state == READY)) {
        processes_list[idx]->state = BLOCKED;
        remove_from_ready_list(processes_list[idx]);
        return 1;
    }
    return -1;
}

int unblock_process(int pid) {
    int idx = foundprocess(pid);
    imprimirVideo("Unblocking process ", 20, BLANCO);
    if (idx != -1 && processes_list[idx]->state == BLOCKED) {
        imprimirVideo("Unblocking process ", 20, BLANCO);
        processes_list[idx]->state = READY;
        add_to_ready_list(processes_list[idx]);
        return 1;
    }
    return -1;
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
    if (current_process) {
        return current_process;
    }
    return NULL;
}

int kill_process(int pid) {
    int idx = foundprocess(pid);
    if (idx == -1)
        return -1;
    freePid(pid);
    p_info* p = processes_list[idx];

    if (p == current_process) {
        p->state = TERMINATED;
        remove_from_ready_list(p);
        
    } else {
        remove_from_processes_list(p);
    }

    return 0;
}


int modify_priority(int pid, int newPriority) {
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

int foundprocess(int pid) {
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
    printFixed("Fd_Table");
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

    for (int i = 0; i < MAX_PROCESSES; i++) {
        p_info *p = processes_list[i];
        if (p && p->waiting_on_child == process->pid) {
            p->waiting_on_child = 0;
            unblock_process(p->pid);
        }
    }

    if (process->parent_pid > 0) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
            p_info *parent = processes_list[i];
            if (parent && parent->pid == process->parent_pid) {
                for (int j = 0; j < parent->children_length; j++) {
                    if (parent->children[j] == process->pid) {
                        parent->children[j] = parent->children[parent->children_length - 1];
                        parent->children[parent->children_length - 1] = 0;
                        parent->children_length--;
                        break;
                    }
                }
                break;
            }
        }
    }
    process->state = TERMINATED;
    remove_from_ready_list(process);
    mm_free(process->stack_top);
    mm_free(process->name);
    mm_free(process->priorityName);
    for (int j = 0; j < process->argc; j++) {
        mm_free(process->argv[j]);
    }
    mm_free(process->argv);

    processes_list[idx] = NULL;
    n_processes--;

    return 0;
}


p_info* get_foreground_process(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        p_info *p = processes_list[i];
        if (p && p->is_foreground) {
            return p;
        }
    }
    return NULL;  
}

p_info* get_process_by_pid(uint16_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] && processes_list[i]->pid == pid) {
            return processes_list[i];
        }
    }
    return NULL;  
}

