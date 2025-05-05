#include <stdint.h>
#include <stddef.h>
#include "test_util.h"
#include "memory/memory_manager.h"
#include "memory/memory_syscalls.h"
#include "syscallsManager.h"
#include "lib.h"
#define MAX_BLOCKS 128
#define MAX_MEMORY 0x4000 // 16KB para test
#define STDOUT 1

// Función auxiliar para convertir int a string
static void int_to_string(int num, char *str)
{
    int i = 0;
    int is_negative = 0;

    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }

    do
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    if (is_negative)
    {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Invertir el string
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Función auxiliar para obtener la longitud de un string
static int string_length(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

typedef struct MM_rq
{
    void *address;
    uint32_t size;
} mm_rq;

void test_mm()
{
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    char buffer[100];

// Imprimir qué administrador de memoria se está usando
#ifdef BUDDY_SYSTEM
    sys_write(STDOUT, "Iniciando test de memoria (Buddy System)\n", 40);
#else
    sys_write(STDOUT, "Iniciando test de memoria (Group Memory Manager)\n", 48);
#endif
    sys_write(STDOUT, "Iniciando test de memoria (Buddy System)\n", 40);
    while (1)
    {
        rq = 0;
        total = 0;
        sys_write(STDOUT, "empieza el malloc\n", 20);
        // Request as many blocks as we can
        while (rq < MAX_BLOCKS && total < MAX_MEMORY)
        {
            mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
            mm_rqs[rq].address = sys_malloc(mm_rqs[rq].size);

            if (mm_rqs[rq].address)
            {
                total += mm_rqs[rq].size;
                rq++;
            }
        }

        sys_write(STDOUT, "empieza el seteo\n", 20);
        // Set
        uint32_t i;
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                memset(mm_rqs[i].address, i, mm_rqs[i].size);

        sys_write(STDOUT, "empieza el check\n", 20);
        // Check
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                {
                    sys_write(STDOUT, "test_mm ERROR\n", 14);
                    return;
                }

        sys_write(STDOUT, "empieza el free\n", 20);
        // Free
        for (i = 0; i < rq; i++)
            if (mm_rqs[i].address)
                sys_free(mm_rqs[i].address);

        sys_write(STDOUT, "empieza el get memory info\n", 20);
        // Imprimir información de memoria
        memory_info_t info;
        sys_get_memory_info(&info);

        sys_write(STDOUT, "Estado de memoria:\n", 19);

        sys_write(STDOUT, "  Bloques totales: ", 19);
        int_to_string(info.block_count, buffer);
        sys_write(STDOUT, buffer, string_length(buffer));
        sys_write(STDOUT, "\n", 1);

        sys_write(STDOUT, "  Bloques libres: ", 18);
        int_to_string(info.free_block_count, buffer);
        sys_write(STDOUT, buffer, string_length(buffer));
        sys_write(STDOUT, "\n", 1);

        sys_write(STDOUT, "  Memoria usada: ", 17);
        int_to_string(info.used_memory, buffer);
        sys_write(STDOUT, buffer, string_length(buffer));
        sys_write(STDOUT, "\n", 1);

        // Esperar un poco antes de la siguiente iteración
        bussy_wait(1000000);
    }
}