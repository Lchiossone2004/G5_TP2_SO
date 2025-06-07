#!/bin/bash

# Por defecto usa group memory manager
MEMORY_MANAGER="group"

# Si se proporciona un argumento, verifica si es "buddy"
if [ $# -eq 1 ]; then
    if [ "$1" = "buddy" ]; then
        MEMORY_MANAGER="buddy"
    elif [ "$1" = "group" ]; then
        MEMORY_MANAGER="group"
    else
        echo "Error: Argumento inválido. Use 'buddy' o 'group'"
        exit 1
    fi
fi
docker start TP2_SO
docker exec -it TP2_SO make clean -C /root/Toolchain
docker exec -it TP2_SO make clean -C /root/
docker exec -it TP2_SO make -C /root/Toolchain
docker exec -it TP2_SO make MEMORY_MANAGER=$MEMORY_MANAGER -C /root/
docker stop TP2_SO  