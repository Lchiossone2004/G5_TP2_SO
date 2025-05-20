#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#define MAX_PROCESSES 10

p_info* processes_list[MAX_PROCESSES];
int n_processes = 0;
int shellCorriendo = 0;

ReadyNode* ready_list = NULL;
ReadyNode* current_node = NULL;
p_info* current_process = NULL;

void* scheduler(void* current_sp) {
    if (!ready_list)
        return current_sp;  // no hay procesos listos

    // Guardar contexto del proceso actual
    if (current_process && current_process->state == RUNNING) {
        current_process->stack_pointer = current_sp;
        current_process->state = READY;
    }

    // Rotar hasta encontrar uno READY
    ReadyNode* start = current_node;
    do {
        current_node = current_node->next;
        if (current_node->process_info->state == READY) {
            current_process = current_node->process_info;
            current_process->state = RUNNING;
            return current_process->stack_pointer;
        }
    } while (current_node != start);

    // Si no hay ninguno en READY, volvemos al actual
    return current_sp;
}



void add_to_ready_list(p_info* process) {
    for (int i = 0; i < process->priority; i++) {
        ReadyNode* node = mm_malloc(sizeof(ReadyNode));
        node->process_info = process;

        if (!ready_list) {
            // Primer nodo
            ready_list = node;
            node->next = node;
            current_node = node;
        } else {
            // Insertar antes del head
            ReadyNode* tail = ready_list;
            while (tail->next != ready_list)
                tail = tail->next;
            tail->next = node;
            node->next = ready_list;
        }
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
            continue; // No avanzar prev
        }

        prev = curr;
        curr = curr->next;
    } while (curr != ready_list);
}

int block_process(uint16_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
    if (processes_list[i] && processes_list[i]->pid == pid && processes_list[i]->state == RUNNING) {
        processes_list[i]->state = BLOCKED;
        return 1;
        //remove_from_ready_list(processes_list[i]);
}
    }
    return 0;
}

int unblock_process(uint16_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
    if (processes_list[i] && processes_list[i]->pid == pid && processes_list[i] == BLOCKED) {
        processes_list[i]->state = READY;
        add_to_ready_list(processes_list[i]);
        return 1;
}
    }
    return 0;
}

void add_to_process_list(p_info* process){
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
p_info* get_current_process(){
    return current_process;
}
int kill_process(uint64_t pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] && processes_list[i]->pid == pid) {
            p_info* p = processes_list[i];
            p->state = TERMINATED;
            remove_from_ready_list(p);

            mm_free(p->stack_base);
            mm_free(p->name);
            mm_free(p);

            processes_list[i] = NULL;
            n_processes--;

            if (p == current_process) {
                current_process = NULL;
            }

            return 1;  
        }
    }
    return 0;  // no se encontró
}

int modify_priority(uint16_t pid, int newPriority) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes_list[i] && processes_list[i]->pid == pid) {
            processes_list[i]->priority = newPriority;
            return 1;
        }
    }
    return 0; //process not found
}
