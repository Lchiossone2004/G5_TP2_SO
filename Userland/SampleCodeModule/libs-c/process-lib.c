#include <process-lib.h>
#include <stdlib.h>
#include <stdint.h>
#include <string-lib.h>
#include <c-lib.h>

extern uint64_t syscall(uint64_t rdi, ...);

int32_t my_create_process( char *name, uint8_t priority, char **argv, void (*function)(uint8_t, char**),
                           int is_foreground, uint8_t argc) {
    return (int32_t) syscall(22, (uint64_t)function, (uint64_t) argc, (uint64_t)argv,
                   (uint64_t)name, (int)priority, (int)is_foreground);
}
int32_t my_kill(int32_t pid) {
    return (int32_t) syscall(23, (uint64_t)pid, 0, 0, 0, 0, 0);
}
int32_t my_block(int32_t pid) {
    return (int32_t) syscall(27, (uint64_t)pid, 0, 0, 0, 0, 0);
}
int32_t my_unblock(int32_t pid) {
    return (int32_t) syscall(28, (uint64_t)pid, 0, 0, 0, 0, 0);
}
int32_t my_nice(int32_t pid, int new_priority) {
    return (int32_t) syscall(26, (uint64_t)pid, (uint64_t)new_priority, 0, 0, 0, 0);
}