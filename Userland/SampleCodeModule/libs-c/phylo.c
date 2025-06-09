#include "../include/c-lib.h"
#include "../include/memory-lib.h"
#include "../include/phylo.h"

#define NO_PID     -1

typedef enum { NONE = 0, THINKING, WAITING, EATING } PHYLO_STATE;

static char state_chars[] = {'0', '.', '.', 'E'};
static char *phylo_names[MAX_DINER] = {
    "Mati", "Prado", "Buela",     "Lolo",
    "Lu", "Tito",     "Fraga",     "Choe",
    "Nicky",      "Caro"
};

static int phylo_pids[MAX_DINER];
static PHYLO_STATE phylo_states[MAX_DINER];
static int phylo_count = 0;

static void print_status() {
    int any = 0;
    for (int i = 0; i < phylo_count; i++) {
        if (phylo_states[i] != NONE) {
            any = 1;
            char buf[2] = { state_chars[phylo_states[i]], '\0' };
            write(buf, 1, 2);
            write(" ", 1, 1);
        }
    }
    if (any) {
        write("\n", 1, 1);
    }
}

static void leave_forks(int idx) {
    usr_sem_wait(SEM_GLOBAL);
    phylo_states[idx] = THINKING;
    int left = (idx + phylo_count - 1) % phylo_count;
    int right = (idx + 1) % phylo_count;
    if (phylo_states[left] == WAITING &&
        phylo_states[(left + phylo_count - 1) % phylo_count] != EATING) {
        phylo_states[left] = EATING;
        usr_sem_post(SEM_FORK(left));
        print_status();
    }
    if (phylo_states[right] == WAITING &&
        phylo_states[(right + 1) % phylo_count] != EATING) {
        phylo_states[right] = EATING;
        usr_sem_post(SEM_FORK(right));
        print_status();
    }
    usr_sem_post(SEM_GLOBAL);
}

static void take_forks(int idx) {
    usr_sem_wait(SEM_GLOBAL);
    phylo_states[idx] = WAITING;
    int left = (idx + phylo_count - 1) % phylo_count;
    int right = (idx + 1) % phylo_count;
    if (phylo_states[left] != EATING && phylo_states[right] != EATING) {
        phylo_states[idx] = EATING;
        usr_sem_post(SEM_FORK(idx));
        print_status();
    }
    usr_sem_post(SEM_GLOBAL);
    usr_sem_wait(SEM_FORK(idx));
}

static void phylo_process(int argc, char *argv[]) {
    int idx = argv[0][0] - '0';
    usr_free(argv[0]);
    usr_free(argv);
    print(phylo_names[idx]);
    print(" joined the table.\n");
    phylo_states[idx] = THINKING;
    while (1) {
        sleep(20);
        take_forks(idx);
        sleep(20);
        leave_forks(idx);
    }
}

static int new_phylo(int idx) {
    usr_sem_wait(SEM_GLOBAL);
    if (usr_sem_open(SEM_FORK(idx), 0) < 0) {
        usr_sem_post(SEM_GLOBAL);
        return -1;
    }

    char **argv = usr_malloc(sizeof(char*) * 2);
    argv[0] = usr_malloc(2);
    argv[0][0] = '0' + idx;
    argv[0][1] = '\0';
    argv[1] = NULL;

    phylo_pids[idx] = usr_create_process((void*)phylo_process, 1, argv, phylo_names[idx], 0, 1);
    if (phylo_pids[idx] < 0) {
        usr_sem_close(SEM_FORK(idx));
        usr_free(argv[0]);
        usr_free(argv);
        usr_sem_post(SEM_GLOBAL);
        return -1;
    }
    phylo_count++;
    print_status();
    usr_sem_post(SEM_GLOBAL);
    return 0;
}

static void remove_phylo(int idx) {
    usr_sem_wait(SEM_GLOBAL);
    print(phylo_names[idx]);
    print(" lefts.\n");
    int left = (idx + phylo_count - 1) % phylo_count;
    int right = (idx + 1) % phylo_count;
    while (phylo_states[left] == EATING && phylo_states[right] == EATING) {
        usr_sem_post(SEM_GLOBAL);
        usr_sem_wait(SEM_FORK(idx));
        usr_sem_wait(SEM_GLOBAL);
    }
    usr_kill(phylo_pids[idx]);
    usr_sem_close(SEM_FORK(idx));
    phylo_pids[idx] = NO_PID;
    phylo_states[idx] = NONE;
    phylo_count--;
    usr_sem_post(SEM_GLOBAL);
}

static void remove_all(int max) {
    for (int i = max - 1; i >= 0; i--) {
        if (phylo_states[i] != NONE) remove_phylo(i);
    }
}

int phylo_main(void) {
    print("The problem of the phylosofers that eat.\n");
    if (usr_sem_open(SEM_GLOBAL, 1) < 0) {
        printErr("Could not start global semaphore.\n");
        return -1;
    }
    for (int i = 0; i < MAX_DINER; i++) {
        phylo_states[i] = NONE;
        phylo_pids[i] = NO_PID;
    }
    for (int i = 0; i < MIN_DINER; i++) {
        if (new_phylo(i) < 0) {
            remove_all(i);
            return -1;
        }
    }

    char cmd;
    while ((cmd = get_char()) != QUIT_VALUE) {
        print("\n");
        if (cmd == ADD_VALUE) {
            if (phylo_count < MAX_DINER) new_phylo(phylo_count);
            else printErr("Max phylosofers reached.\n");
        } else if (cmd == REMOVE_VALUE) {
            if (phylo_count > MIN_DINER) remove_phylo(phylo_count - 1);
            else printErr("Min phylosofers reached.\n");
        }
    }
    remove_all(phylo_count);
    usr_sem_close(SEM_GLOBAL);
    print("\n");
    return 0;
}

