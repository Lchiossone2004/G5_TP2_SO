#include <stdint.h>
#include <font.h>

static int flag = 0; //arranca en 0 porque es el zoom default

//newBitMap tiene que tener el tamaño correcto OJO!!!! (8*n, 128*n)
void expand(uint8_t * newBitMap) {
  if(flag < 0) {
        reduceN(newBitMap, - flag - 1); //si esta en -2 y quiero agrandarlo, reduzco en 1
    } else {
        expandN(newBitMap, flag + 1);
    }
    flag++;
}

void reduce(uint8_t * newBitMap) {
    if (flag > 0) {
        expandN(newBitMap, flag - 1);
    } else {
        reduceN(newBitMap, - flag + 1);
    }
    flag--;
}

void expandN(uint8_t * newBitMap, int n) {
        for(int i = 0; i < 128 * n; i+n) { //para cada fila en newBitMap
        for(int j = 0; j < 128; j++) { //para cada fila en font
            expandirFila(font[j], newBitMap[i], n); //expando la fila de font en la fila de newBitMap
            for(int k = 1; k < n; k++) { 
            newBitMap[i + k] = newBitMap[i]; //duplico la columna n veces
            }
        }
    }
}
void expandirFila(uint8_t * original, uint16_t *expandido, int n) {
    int idx = 0;
    // recorro cada byte en la fila
    for (int i = 0; i < 8; i++) {
        uint8_t byte = original[i];
        // recorro los bits del byte
        for (int bitPos = 7; bitPos >= 0; bitPos--) {
            uint16_t bit = (byte >> bitPos) & 1; // extraigo el bit
            for (int j = 0; j < n; j++) {
                expandido[idx++] = bit; // lo duplico n veces y lo guardo en idx
            }
        }
    }
}
  
void reduceN(uint8_t * newBitMap, int n) {

}