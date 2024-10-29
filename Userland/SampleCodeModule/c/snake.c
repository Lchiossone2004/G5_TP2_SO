

#include "../include/snake.h" 
#include <stdint.h>
extern uint64_t syscall(uint64_t rdi, ...);

typedef struct {
	int pos_x;
	int pos_y;
    int direc_x;
    int direc_y;
    int points;
    int len;
    uint32_t color;
} Snakepos;

#define COLOR_1 0x00b6e2f0
#define COLOR_2 0x009de0f5
#define REC_ANCHO 32
#define REC_LARGO 32
#define BORDER_X 1016
#define BORDER_Y 768
#define REC_X_FIL BORDER_X / REC_ANCHO
#define REC_X_COL BORDER_Y / REC_LARGO
#define MAX_SNAKE 10

static char snake_is_active=1;
Snakepos snake1[MAX_SNAKE]; 
Snakepos snake2[MAX_SNAKE];
Snakepos circle;

//inicializa la snake desde el principio del canvas con dirección hacia la derecha
void iniSnake1() {
    snake1->points = 0;
    snake1->direc_x = REC_ANCHO;
    snake1->direc_y = 0;
    snake1->len = 4;
    snake1->color = 0x0042f557;
    for (int i = 0; i < snake1->len; i++) {
        snake1[i].pos_y = 0;
        snake1[i].pos_x = (1 + i) * REC_ANCHO; 
    }
}
void iniSnake2() {
    snake2->points = 0;
    snake2->direc_x = 0;
    snake2->direc_y = -REC_LARGO;
    snake2->len = 4;
    snake2->color = 0x00f54290;
    for(int i = 0; i < snake2->len; i++) {
        snake2[i].pos_x = BORDER_X - 2 * REC_ANCHO;
        snake2[i].pos_y = BORDER_Y - (3+i) * REC_LARGO;
    }
}

void putRectangle(int posx, int posy, uint32_t color) {
	for(int i = posx; i < posx+REC_ANCHO; i++) {
		for(int j = posy; j < posy+REC_LARGO; j++) {
			syscall(10,i, j, color);
		}
	}
}

void snakeCanvas() {
	int flag = 0;
	for(int i = 0; i < BORDER_X + 1; i += REC_ANCHO) {
		flag = !flag;
		for(int j = 0; j < BORDER_Y; j += REC_LARGO) {
			if(flag) {
				putRectangle(i, j, COLOR_1);
			} else {
				putRectangle(i,j,COLOR_2);
			}
			flag = !flag;
		}
	}
}

void putSnake(Snakepos snake[]) {
    for(int i = 0; i < snake->len; i++) {
        putRectangle(snake[i].pos_x, snake[i].pos_y, snake->color);
    }
}

void moveSnake(Snakepos snake[]) {
    int l = (snake->len) -1;
    delete(snake[0].pos_x, snake[0].pos_y);
    //va moviendo las partes hacia adelante
    for(int i = 0; i < l; i++) {
        snake[i].pos_x = snake[i+1].pos_x;
        snake[i].pos_y = snake[i+1].pos_y;
    }
    //mueve la cabeza en su direccion
    snake[l].pos_x += snake->direc_x;
    snake[l].pos_y += snake->direc_y;
    putSnake(snake);
    
}

void putCircle(int posx, int posy, uint32_t color) {
    int centerX = posx + REC_ANCHO / 2;  
    int centerY = posy + REC_LARGO / 2;  
    int radius = (REC_ANCHO < REC_LARGO ? REC_ANCHO : REC_LARGO) / 4;  
    for (int i = posx; i < posx + REC_ANCHO; i++) {
        for (int j = posy; j < posy + REC_LARGO; j++) {
            int dx = i - centerX;  
            int dy = j - centerY;  
            if (dx * dx + dy * dy <= radius * radius) {  
                syscall(10,i, j, color);
            }
        }
    }
}
int isPositionOccupied(int x, int y) {
    for (int i = 0; i < snake1->len; i++) {
        if (snake1[i].pos_x == x && snake1[i].pos_y == y) {
            return 1;
        }
    }
    for(int i = 0;i<snake2->len; i++) {
        if(snake2[i].pos_x == x && snake2[i].pos_y == y) {
            return 1;
        }
    }
    return 0;
}

void putRandomCircle() {
    int ran;
    int colApple, filApple;
    int posX, posY;

    do {
        syscall(15, &ran); 
        colApple = ran % REC_X_FIL;
        filApple = (ran / REC_X_FIL) % REC_X_COL;

        posX = colApple * REC_ANCHO;
        posY = filApple * REC_LARGO;

    } while (isPositionOccupied(posX, posY));  // Repite si la posición está ocupada

    circle.pos_x = posX;
    circle.pos_y = posY;
    putCircle(circle.pos_x, circle.pos_y, 0x00FA0202);
}

int isPair(int pos) {
	return pos % 2 == 0;
}

void deleteCircle() {
	delete(circle.pos_x, circle.pos_y);
}
void delete(int pos_x, int pos_y) {
    if((isPair(pos_x / REC_ANCHO) && isPair(pos_y / REC_LARGO))||(!isPair(pos_x / REC_ANCHO) && !isPair(pos_y/REC_LARGO))) {
		putRectangle(pos_x, pos_y, COLOR_1);
	} else {
		putRectangle(pos_x,pos_y, COLOR_2);
	}
}


int isSnakeinPos(Snakepos pos, Snakepos snake[]) {
    int l = snake->len-1;
    return snake[l].pos_x == pos.pos_x && snake[l].pos_y == pos.pos_y;
}

void pointEarned(Snakepos snake[]) {//CUANDO CRECE, LO HACE PARA ADELANTE, fijarse si podemos hace que mantenga la cola por una unidad de tiempo
    if(snake->len < MAX_SNAKE) {
        snake->len++;
        snake[snake->len-1].pos_x = snake[snake->len-2].pos_x + snake->direc_x;
        snake[snake->len-1].pos_y = snake[snake->len-2].pos_y + snake->direc_y;
        putSnake(snake);
    }
    snake->points++;
    deleteCircle();
    putRandomCircle(); 

}

void direcSnake1(char key) {
    int newX = snake1->direc_x;
    int newY = snake1->direc_y;
    switch(key) {
        case 0x11: newX = 0; newY = -REC_LARGO; break;//w
        case 0x1F: newX = 0; newY = REC_LARGO; break;//s
        case 0x1E: newX = -REC_ANCHO; newY = 0; break;//d
        case 0x20: newX = REC_ANCHO; newY = 0; break;//a
        default: return;
    }
    changeDir(newX, newY,snake1);
}
void direcSnake2(char key) {
    int newX = snake2->direc_x;
    int newY = snake2->direc_y;
    switch(key) {
        case 0x17: newX = 0; newY = -REC_LARGO; break;//i
        case 0x25: newX = 0; newY = REC_LARGO; break;//k
        case 0x24: newX = -REC_ANCHO; newY = 0; break;//j
        case 0x26: newX = REC_ANCHO; newY = 0; break;//l
        default: return;
    }
    changeDir(newX, newY,snake2);
}
void changeDir(int newX, int newY, Snakepos snake[]) {
    if ((newX != -(snake->direc_x)) && (newY != -(snake->direc_y))) {
        snake->direc_x = newX;
        snake->direc_y = newY;
    }
}
int checkCollision(Snakepos snake[]) {
    int len = snake->len;
   
    // choca con un borde
    if (snake[len - 1].pos_x < 0 || snake[len - 1].pos_x >= BORDER_X ||snake[len - 1].pos_y < 0 || snake[len - 1].pos_y >= BORDER_Y) {
        return 1;
    }
    //se choca a ella misma 
     for(int i = 0; i < len -1; i++) {
         if(snake[i].pos_x == snake[len-1].pos_x && snake[i].pos_y == snake[len-1].pos_y) {
             return 1;
         }
     }
    return 0;
}

void endGame() {
    snake_is_active = 0;
    char * phrase= "Score player one:";
    //char *phrase2 = "Score player two:";
    //char pts2[2];
    char pts[2];
    pts[0] = snake1->points + '0';
    pts[1] = '\0';
   // pts2[0] = snake2->points + '0';
   // pts2[1] = "\0";
    syscall(9, 1);  // Limpia la pantalla usando una llamada al sistema
    syscall(6,1); //hace zoom asi se imprime el msj mas grande
    syscall(4,1,phrase,17); //imprime el msj que perdiste
    syscall(4,1,pts,1);
    //syscall(4,1,phrase2,17); //imprime el msj que perdiste
    //syscall(4,1,pts2,1);
    syscall(8,2);
    syscall(7,1);
    syscall(9,1);

}

void playSnake() {
    snakeCanvas();
    iniSnake1();
    putSnake(snake1);
    putRandomCircle();
    int exitPressed;
    int direction;
    while(snake_is_active){
        syscall(14, &endGame);
        syscall(14, &direction);
        direcSnake1(direction);
        moveSnake(snake1);
        direction=0;
        syscall(8,1);
        if(isSnakeinPos(circle, snake1)) {
            pointEarned(snake1);
        }
        if(0x30==exitPressed || checkCollision(snake1)){
            endGame();
        }
    }
}

