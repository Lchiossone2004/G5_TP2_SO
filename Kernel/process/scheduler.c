#include "../include/scheduler.h"
#include "../memory/memory_manager.h"
#define MAX_PROCESSES 10

p_info* processes_list[MAX_PROCESSES];
int n_processes = 0;
int shellCorriendo = 0;

ReadyNode* ready_list = NULL;
ReadyNode* current_node = NULL;
p_info* current_process = NULL;

void* scheduler(void* current_sp){
    // if (current_process) {
    //     // Guardar contexto del proceso actual
    //     current_process->stack_pointer = current_sp;
    //     if (current_process->state == RUNNING)
    //         current_process->state = READY;
    // }

    // // Si es la primera vez, elegimos el primer proceso listo (bootstrap)
    // if (!current_node)
    //     current_node = ready_list;

    // if (!current_node)
    //     return current_sp; // No hay ningún proceso cargado aún

    // ReadyNode* start = current_node;
    // do {
    //     current_node = current_node->next;
    //     if (current_node->process_info->state == READY) {
    //         p_info* next = current_node->process_info;
    //         next->state = RUNNING;
    //         current_process = next;
    //         return next->stack_pointer;
    //     }
    // } while (current_node != start);
    if(n_processes == 0 || shellCorriendo > 0)
    return current_sp;
    shellCorriendo = 1;
    return processes_list[0]->stack_pointer;
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

void block_process(p_info* process) {
    process->state = BLOCKED;
    remove_from_ready_list(process);
}

void unblock_process(p_info* process) {
    process->state = READY;
    add_to_ready_list(process);
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