#include <stdint.h>
#include <stdlib.h>
int32_t my_create_process( char *name, uint8_t priority, char **argv, void (*function)(uint8_t, char**),int is_foreground, uint8_t argc);
int32_t my_kill(int32_t pid);
int32_t my_block(int32_t pid);
int32_t my_unblock(int32_t pid);
int32_t my_nice(int32_t pid, int new_priority);