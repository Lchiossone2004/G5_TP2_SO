

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
#define MAX_SNAKE 120

static char snake_is_active=1;
Snakepos snake1[MAX_SNAKE]; 
Snakepos snake2[MAX_SNAKE];
Snakepos circle;

static int keysPressed[100]={0};


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
        snake2[i].pos_x = (BORDER_X / 2 / REC_ANCHO) * 2 * REC_ANCHO;
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
    int l = snake->len - 1;

    delete(snake[0].pos_x, snake[0].pos_y); // Limpia la cola

    // Mueve los segmentos de la snake
    for (int i = 0; i < l; i++) {
        snake[i].pos_x = snake[i + 1].pos_x;
        snake[i].pos_y = snake[i + 1].pos_y;
    }

    // Mueve la cabeza en la dirección actual
    snake[l].pos_x += snake->direc_x;
    snake[l].pos_y += snake->direc_y;

    putSnake(snake); // Dibuja la snake después de mover
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
int isPositionOccupied(Snakepos pos) {
    if(isSnakeinPos(pos,snake1) || isSnakeinPos(pos,snake2)) {
        return 1;
    }
    return 0;
}

void putRandomCircle() {
    int ran;
    int colApple, filApple;
    Snakepos pos;

    do {
        syscall(15, &ran); 
        colApple = ran % REC_X_FIL;
        filApple = (ran / REC_X_FIL) % REC_X_COL;

        pos.pos_x = colApple * REC_ANCHO;
        pos.pos_y = filApple * REC_LARGO;

    } while (isPositionOccupied(pos));  // Repite si la posición está ocupada

    circle.pos_x = pos.pos_x;
    circle.pos_y = pos.pos_y;
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

void pointEarned(Snakepos snake[]){  
    syscall(11,0);
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
void direcSnake(char cantPlayers) {
    int newX1 = snake1->direc_x;
    int newY1 = snake1->direc_y;
    int newX2 = 0, newY2 = 0;

    if (cantPlayers == 2) {
        newX2 = snake2->direc_x;
        newY2 = snake2->direc_y;
    }

    for (int i = 0; keysPressed[i]; i++) {
        switch (keysPressed[i]) {
            case 0x11: newX1 = 0; newY1 = -REC_LARGO; break; // W
            case 0x1F: newX1 = 0; newY1 = REC_LARGO; break;  // S
            case 0x1E: newX1 = -REC_ANCHO; newY1 = 0; break; // A
            case 0x20: newX1 = REC_ANCHO; newY1 = 0; break;  // D
            default: break;
        }

        if (cantPlayers == 2) {
            switch (keysPressed[i]) {
                case 0x17: newX2 = 0; newY2 = -REC_LARGO; break; // I
                case 0x25: newX2 = 0; newY2 = REC_LARGO; break;  // K
                case 0x24: newX2 = -REC_ANCHO; newY2 = 0; break; // J
                case 0x26: newX2 = REC_ANCHO; newY2 = 0; break;  // L
                default: break;
            }
        }
        keysPressed[i] = 0;
    }

    changeDir(newX1, newY1, snake1);
    if (cantPlayers == 2) {
        changeDir(newX2, newY2, snake2);
    }
}

void changeDir(int newX, int newY, Snakepos snake[]) {
    if ((newX != -(snake->direc_x)) && (newY != -(snake->direc_y))) {
        snake->direc_x = newX;
        snake->direc_y = newY;
    }
}
int checkCollision(Snakepos snake[], Snakepos othersnake[]) {
    int len = snake->len;
   
    // choca con un borde
    if (snake[len - 1].pos_x < 0 || snake[len - 1].pos_x >= BORDER_X ||snake[len - 1].pos_y < 0 || snake[len - 1].pos_y >= BORDER_Y) {
        syscall(11,1);
        return 1;
    }
    //se choca a ella misma 
     for(int i = 0; i < len -1; i++) {
         if(snake[i].pos_x == snake[len-1].pos_x && snake[i].pos_y == snake[len-1].pos_y) {
            syscall(11,1);
             return 1;
         }
     }
     //se choca con la otra
     for(int i = 0; i < othersnake->len; i++) {
        if(othersnake[i].pos_x == snake[len-1].pos_x && othersnake[i].pos_y == snake[len-1].pos_y) {
            syscall(11,1);
            return 1;
        }
     }
    return 0;
}

void endGameOnePlayer() {
    snake_is_active = 0;
    char * phrase= "SCORE: ";
    char pts[2];
    pts[0] = snake1->points + '0';
    pts[1] = '\0';
    syscall(9, 1);  // Limpia la pantalla usando una llamada al sistema
    syscall(6,1); //hace zoom asi se imprime el msj mas grande
    syscall(4,1,phrase,7); //imprime el msj 
    syscall(4,1,pts,1);
    syscall(8,36);
    syscall(7,1);
    syscall(9,1);
    return;
}
void endGameTwoPlayers(int n) {
    snake_is_active = 0;
    char * p1 = "PLAYER ONE SCORE: ";
    char *p2 = "PLAYER TWO SCORE: ";
    char *p3 = "WINNER: PLAYER ";
    char ptsO[2];
    ptsO[0] = snake1->points + '0';
    ptsO[1] = '\0';
    char ptsT[2];
    ptsT[0] = snake2->points + '0';
    ptsT[1] = '\0';
    char pts3[2];
    pts3[0] = n + '0';
    pts3[1] = '\0';
    syscall(9, 1);  // Limpia la pantalla usando una llamada al sistema
    syscall(6,1); //hace zoom asi se imprime el msj mas grande
    syscall(4,1,p1,18);
    syscall(4,1,ptsO,1);
    syscall(5);
    syscall(4,1,p2,18); 
    syscall(4,1,ptsT,1);
    syscall(5);
    if(n == 0) {
        return;
    }
    syscall(4,1,p3,15);
    syscall(4,1,pts3,1);
    syscall(8,36);
    syscall(7,1);
    syscall(9,1);
    return;
}

void playSnake() {
    int exitPressed;
    
    char count=0;
    while(snake_is_active){
    syscall(14, &exitPressed);
        syscall(14, keysPressed+(count++));
        direcSnake(1);
        moveSnake(snake1);
        syscall(8,8);
        count=0;
        if(isSnakeinPos(circle, snake1)) {
            pointEarned(snake1);
        }
        if(0x2D==exitPressed || checkCollision(snake1, snake2)){
            endGameOnePlayer();
        }
    }
}
void play2Snakes() {
    iniSnake2();
    putSnake(snake2);
    int exitPressed;
    char count=0;
    while(snake_is_active){
        syscall(14, &exitPressed);
        syscall(14, keysPressed+(count++));
        direcSnake(2);
        moveSnake(snake1);
        moveSnake(snake2);
        syscall(8,8);
        count=0;
        if(isSnakeinPos(circle, snake1)) {
            pointEarned(snake1);
        }
        if(isSnakeinPos(circle, snake2)) {
            pointEarned(snake2);
        }
        if(0x2D == exitPressed) {
            endGameTwoPlayers(0);
        }
        if(checkCollision(snake1, snake2)) {
            endGameTwoPlayers(2);
        }
        if(checkCollision(snake2, snake1)) {
            endGameTwoPlayers(1);
        }


}
}
void play(char players) {
    snake_is_active = 1;
    snakeCanvas();
    iniSnake1();
    putSnake(snake1);
    putRandomCircle();
    if(players == '1') {
        playSnake();
        return;
    }
    play2Snakes();
    snake_is_active=1;
}