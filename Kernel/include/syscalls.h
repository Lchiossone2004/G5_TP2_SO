#include <stdint.h>
#include <stdlib.h>

size_t sys_read(unsigned int fd, char *buffer, size_t count);
size_t sys_write(unsigned int fd, const char *buffer, size_t count);
