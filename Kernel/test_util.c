#include <stdint.h>
#include "test_util.h"
#include "syscallsManager.h"

#define STDOUT 1

static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
    uint32_t num = GetUint();
    return num % max;
}

uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
    uint8_t *memory = (uint8_t *)start;
    uint32_t i;

    for (i = 0; i < size; i++) {
        if (memory[i] != value)
            return 0;
    }
    return 1;
}

int64_t satoi(char *str) {
    int64_t num = 0;
    int8_t sign = 1;
    
    if (*str == '-') {
        sign = -1;
        str++;
    }
    
    while (*str) {
        if (*str < '0' || *str > '9')
            return 0;
        num = num * 10 + (*str - '0');
        str++;
    }
    
    return num * sign;
}

void bussy_wait(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++);
}

void endless_loop() {
    while(1);
}

void endless_loop_print(uint64_t wait) {
    int64_t i;
    while(1) {
        if (wait != 0) {
            for (i = 0; i < wait; i++);
        }
    }
}

void print_num(uint64_t num) {
    char buffer[21];
    char *p = buffer;
    
    if (num == 0) {
        sys_write(STDOUT, "0", 1);
        return;
    }
    
    while (num > 0) {
        *p++ = '0' + (num % 10);
        num /= 10;
    }
    
    while (p > buffer) {
        p--;
        char c = *p;
        sys_write(STDOUT, &c, 1);
    }
} 