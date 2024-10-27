

#include "../include/snake.h" 
#include <stdint.h>

typedef struct {
	int pos_x;
	int pos_y;
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

static int direc_x = REC_ANCHO; // dirección en x. arranca por default hacia la derecha y se mueve en cuadrados
static int direc_y = 0; //como arranca por default hacia la derecha el movimiento en y default es 0
static int len = 4; //tamaño inicial: 4 cuadraditos
static int points = 0;

Snakepos snake[MAX_SNAKE]; 
Snakepos circle;

//inicializa la snake desde el principio del canvas con dirección hacia la derecha
void iniSnake() {
    for(int i = 0; i < len; i++) {
        snake[i].pos_y = 0;
        snake[i].pos_x = (len - i)*REC_ANCHO;
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

void putSnake() {
    for(int i = 0; i < len; i++) {
        putRectangle(snake[i].pos_x, snake[i].pos_y, 0x0002FA0F);
    }
}

void moveSnake() {
    delete(snake[0].pos_x, snake[0].pos_y);
    for(int i = 0; i < len-1; i++) {
        snake[i].pos_x = snake[i+1].pos_x;
        snake[i].pos_y = snake[i+1].pos_y;
    } 
        snake[len-1].pos_x += direc_x;
        snake[len-1].pos_y += direc_y;
        putSnake();
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

//imprime un circulo en una pos random (seria la manzana) y guarda la posición
void putRandomCircle() {
    int min = syscall(12); 
    int sec = syscall(11); 

    int area = REC_X_FIL * REC_X_COL;
    int idx = (min * 60 + sec) % area;
    int row = idx / REC_X_COL;
    int col = idx % REC_X_COL;          
    circle.pos_x = col * REC_ANCHO; 
    circle.pos_y = col * REC_LARGO;
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

void changeDir(int newX, int newY) {
    if((newX != 0 && snake->pos_x != 0) || (newY != 0 && snake->pos_y != 0)) { //chequear esto
        return;
    }
    snake->pos_x = newX;
    snake ->pos_y = newY;
}

int isSnakeinPos(Snakepos pos) {
    return snake[len-1].pos_x == pos.pos_x && snake[len-1].pos_y == pos.pos_y;
}

void pointEarned() {
    if(len < MAX_SNAKE) {
        len++;
        snake[len-1].pos_x = snake[len-2].pos_x + direc_x;
        snake[len-1].pos_y = snake[len-2].pos_y + direc_y;
        //cnequear esto pero creo q va ok
        putSnake();
    }
    points++;
    deleteCircle();
    putRandomCircle(); //esto habria q chequear q no se cree donde esta la snake

}

void playSnake() {
    snakeCanvas();
    iniSnake();
    putSnake();
    putRandomCircle();

}

