#include <stdint.h>

#define SEM_MAX 128
#define MAX_PROCESSES 10
typedef uint16_t pid_t;

typedef struct semaphore {
    int8_t value;
    int8_t lock;
    pid_t waiting[MAX_PROCESSES];
    int head;
    int tail; 
    int count;
} semaphore;

void init_semaphores(void);

int8_t sem_open(int8_t id, int8_t initial_value);
int8_t sem_close(int8_t id);
int8_t sem_wait(int8_t id);
int8_t sem_post(int8_t id);
int8_t sem_get_value(int8_t id);