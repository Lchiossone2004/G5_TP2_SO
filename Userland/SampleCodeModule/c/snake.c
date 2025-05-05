#include "../include/snake.h" 
#include <stdint.h>
#include <libc.h>
extern uint64_t syscall(uint64_t rdi, ...);

#define COLOR_1 0x00b6e2f0
#define COLOR_2 0x009de0f5
#define REC_ANCHO 32
#define REC_LARGO 32
#define BORDER_X_INI 0
#define BORDER_Y_INI 96
#define BORDER_X_FIN 1016 
#define BORDER_Y_FIN 768 
#define START_CANVAS_X BORDER_X_INI + REC_ANCHO
#define START_CANVAS_Y BORDER_Y_INI + REC_LARGO
#define END_CANVAS_X BORDER_X_FIN - REC_ANCHO
#define END_CANVAS_Y BORDER_Y_FIN - REC_LARGO
#define REC_X_FIL BORDER_X_FIN / REC_ANCHO
#define REC_X_COL BORDER_Y_FIN / REC_LARGO
#define MAX_SNAKE REC_X_FIL*REC_X_COL

static char pointsStr1[5]; //como la maxima cantidad de puntos posible es 1024 (por el espacio) no hace falta mas de 4 chars para guardar todos los puntos, el 5 es el /0
static char pointsStr2[5];
typedef struct {
	int pos_x;
	int pos_y;
    int direc_x;
    int direc_y;
    int points;
    int len;
    int extraLen; // Agregado para gestionar el incremento en la siguiente unidad de tiempo
    uint32_t color;
} Snakepos;

Snakepos snake1[MAX_SNAKE]; 
Snakepos snake2[MAX_SNAKE];
Snakepos circle;

void resetGameState() {
    // Reinicia la vibora 1
    for (int i = 0; i < MAX_SNAKE; i++) {
        snake1[i].pos_x = 0;
        snake1[i].pos_y = 0;
        snake1[i].direc_x = 0;
        snake1[i].direc_y = 0;
        snake1[i].points = 0;
        snake1[i].len = 0;
        snake1[i].extraLen = 0;
        snake1[i].color = 0x0042f557;// Color
    }
    // Reinicia la vibora 2
    for (int i = 0; i < MAX_SNAKE; i++) {
        snake2[i].pos_x = 0;
        snake2[i].pos_y = 0;
        snake2[i].direc_x = 0;
        snake2[i].direc_y = 0;
        snake2[i].points = 0;
        snake2[i].len = 0;
        snake2[i].extraLen = 0;
        snake2[i].color = 0x00f54290;
    }
    // Reinicia la manzana
    circle.pos_x = 0;
    circle.pos_y = 0;
    circle.direc_x = 0;
    circle.direc_y = 0;
    circle.points = 0;
    circle.len = 0;
    circle.extraLen = 0;
    circle.color = 0x00FA0202;
    // Limpia pantalla
    syscall(9, STDOUT);
}

//inicializa la snake desde el principio del canvas con dirección hacia la derecha
void iniSnake1() {
    snake1->points = 0;
    snake1->direc_x = REC_ANCHO;
    snake1->direc_y = 0;
    snake1->len = 4;
    snake1->extraLen = 0;
    snake1->color = 0x0042f557;
    for (int i = 0; i < snake1->len; i++) { 
        snake1[i].pos_y = START_CANVAS_Y;
        snake1[i].pos_x = (1 + i) * REC_ANCHO; 
    }
    return;
}

void iniSnake2() {
    snake2->points = 0;
    snake2->direc_x = 0;
    snake2->direc_y = -REC_LARGO;
    snake2->len = 4;
    snake2->extraLen = 0;
    snake2->color = 0x00f54290;
    for(int i = 0; i < snake2->len; i++) {
        snake2[i].pos_x = (BORDER_X_FIN / 2 / REC_ANCHO) * 2 * REC_ANCHO;
        snake2[i].pos_y = BORDER_Y_FIN - (3+i) * REC_LARGO;
    }
    return;
}

void putRectangle(int posx, int posy, uint32_t color) {
    for (int i = posx; i < posx + REC_ANCHO; i++) {
        for (int j = posy; j < posy + REC_LARGO; j++) {
            syscall(10, i, j, color, STDOUT);
        }
    }
}

int isPair(int pos) {
	return pos % 2 == 0;
}
void putMap(int pos_x, int pos_y) {
    if((isPair(pos_x / REC_ANCHO) && isPair(pos_y / REC_LARGO))||(!isPair(pos_x / REC_ANCHO) && !isPair(pos_y/REC_LARGO))) {
		putRectangle(pos_x, pos_y, COLOR_1);
	} else {
		putRectangle(pos_x,pos_y, COLOR_2);
	}
    return;
}
void snakeCanvas() {
	 for (int i = START_CANVAS_X; i < END_CANVAS_X; i += REC_ANCHO) {
        for (int j = START_CANVAS_Y; j < END_CANVAS_Y; j += REC_LARGO) {
            putMap(i, j);
        }
    }
    return;
}

void putSnake(Snakepos snake[]) {
    for(int i = 0; i < snake->len; i++) {
        putRectangle(snake[i].pos_x, snake[i].pos_y, snake->color);
    }
    
    return;
}

void moveSnake(Snakepos snake[]) {
    int l = snake->len-1;
    putMap(snake[0].pos_x, snake[0].pos_y);// Limpia la cola
    if (snake->extraLen){
        snake->len++; // Incrementa la longitud de la viborita
        l++;
        snake->extraLen=0; // Decrementa longitud extra
        for (int i = 0; i < l-1; i++){
            snake[i].pos_x = snake[i + 1].pos_x+snake[i + 1].direc_x;
            snake[i].pos_y = snake[i + 1].pos_y+snake[i + 1].direc_y;
        }
        // Mueve la cabecita en la direccion mandada
        snake[l].pos_x += snake[l-1].pos_x+snake->direc_x;
        snake[l].pos_y += snake[l-1].pos_y+snake->direc_y;
    }else {
        // Mueve el cuerpo de la snake
        for (int i = 0; i < l; i++) {
            snake[i].pos_x = snake[i + 1].pos_x;
            snake[i].pos_y = snake[i + 1].pos_y;
        }
        // Mueve la cabeza en la dirección actual
        snake[l].pos_x += snake->direc_x;
        snake[l].pos_y += snake->direc_y;
    }
    if(snake[l].pos_x < START_CANVAS_X ||snake[l].pos_x >= END_CANVAS_X ||snake[l].pos_y < START_CANVAS_Y ||snake[l].pos_y >= END_CANVAS_Y ) {
       
        return;
    }
    putSnake(snake); // Dibuja la snake después de mover
    return;
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
                syscall(10,i, j, color, STDOUT);
            }
        }
    }
    return;
}
int isSnakeinPos(Snakepos pos, Snakepos snake[]) {
    int l = snake->len;
    for (int i = 0; i < l; i++){
        if(snake[i].pos_x == pos.pos_x && snake[i].pos_y == pos.pos_y){
            return 1;
        }
    }
    return 0;
}
int isPositionOccupied(Snakepos pos) {
    if(isSnakeinPos(pos,snake1) || isSnakeinPos(pos,snake2)) {
        return 1;
    }
    return 0;
}
int posOutOfBounds(Snakepos pos) {
    return pos.pos_x >= END_CANVAS_X || pos.pos_x < START_CANVAS_X || pos.pos_y >= END_CANVAS_Y || pos.pos_y < START_CANVAS_Y;
}
void putRandomCircle() {
    int ran;
    int colApple, filApple;
    Snakepos pos;
    do {
        syscall(15, &ran); 
        colApple = ran % (BORDER_X_FIN / REC_ANCHO);
        filApple = ran % ((BORDER_Y_FIN-BORDER_Y_INI )/ REC_LARGO);
        pos.pos_x = colApple * REC_ANCHO;
        pos.pos_y = filApple * REC_LARGO + BORDER_Y_INI;
    } while (isPositionOccupied(pos)|| posOutOfBounds(pos) );  // Repite si la vibora esta ahi
    circle.pos_x = pos.pos_x;
    circle.pos_y = pos.pos_y;
    putCircle(circle.pos_x, circle.pos_y, 0x00FA0202);//coloca efectivamente la manzana
    return;
}


void deleteCircle() {
	putMap(circle.pos_x, circle.pos_y);
    return;
}


void pointEarned(Snakepos snake[]) {
    syscall(11, 0, STDOUT);//beep
    // marca q gano un punto
    if (snake->len < MAX_SNAKE) {
        snake->extraLen=1;
    }
    snake->points++;
    deleteCircle();
    putRandomCircle();
    return;
}

void intToStr(int num, char* str) {
    int i = 0;
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    str[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
    return;
}
int digitCount(int number) {
    int count = 0;
    do {
        count++;
        number /= 10;
    } while (number != 0);
    return count;
}

void printPoints() {
    syscall(17,124);
    print("Player 1:", 9);
    intToStr(snake1->points, pointsStr1);
    print(pointsStr1, digitCount(snake1->points));
    nlPrint();
    print("Player 2:", 9);
    intToStr(snake2->points, pointsStr2);
    print(pointsStr2, digitCount(snake2->points));
}
void changeDir(int newX, int newY, Snakepos snake[]) {
    if ((newX != -(snake->direc_x)) && (newY != -(snake->direc_y))) {
        snake->direc_x = newX;
        snake->direc_y = newY;
    }
    return;
}


void direcSnake(char cantPlayers, char keyPressed) {
    int newX1 = snake1->direc_x;
    int newY1 = snake1->direc_y;
    int newX2 = 0, newY2 = 0;
    if (cantPlayers == '2') {
        newX2 = snake2->direc_x;
        newY2 = snake2->direc_y;
    }
    switch (keyPressed) {
        case 'W':;
        case 'w': newX1 = 0; newY1 = -REC_LARGO; break;
        case 'S':;
        case 's': newX1 = 0; newY1 = REC_LARGO; break;
        case 'A':;
        case 'a': newX1 = -REC_ANCHO; newY1 = 0; break;
        case 'D':;
        case 'd': newX1 = REC_ANCHO; newY1 = 0; break;
        default:break;;
        }
    if (cantPlayers == '2') {
        switch (keyPressed) {
            case 'I':;
            case 'i': newX2 = 0; newY2 = -REC_LARGO; break;
            case 'K':;
            case 'k': newX2 = 0; newY2 = REC_LARGO; break;
            case 'J':;
            case 'j': newX2 = -REC_ANCHO; newY2 = 0; break;
            case 'L':;
            case 'l': newX2 = REC_ANCHO; newY2 = 0; break;
            default:break;;
        }
    }
    changeDir(newX1, newY1, snake1);
    if (cantPlayers == '2') {
        changeDir(newX2, newY2, snake2);
    }
    return;
}

int checkSelfCollision(Snakepos snake[]) {
  int len = snake->len;
    // choca con un borde
    if (snake[len - 1].pos_x < BORDER_X_INI + REC_ANCHO|| snake[len - 1].pos_x >= BORDER_X_FIN - REC_ANCHO|| snake[len - 1].pos_y < BORDER_Y_INI + REC_LARGO || snake[len - 1].pos_y >= BORDER_Y_FIN -REC_LARGO  ) {
        return 1;
    }
    // se choca a ella misma
    for(int i = 0; i < len - 1; i++) {
        if(snake[i].pos_x == snake[len - 1].pos_x && snake[i].pos_y == snake[len - 1].pos_y) {
            return 1;
        }
    }
    return 0;
}
int checkOtherSnakeCollission() {
    int len = snake1->len;
    int otherlen = snake2->len; 
    //de empate (cabeza con cabeza)
    if(snake1[len - 1].pos_x == snake2[otherlen - 1].pos_x && snake1[len - 1].pos_y == snake2[otherlen - 1].pos_y && (snake1->direc_x== -snake2->direc_x ||snake1->direc_y== -snake2->direc_y ) ) {
        return 3;
    }
 // se choca la snake 1 con la snake 2
    for(int i = 0; i < otherlen; i++) {
        if(snake2[i].pos_x == snake1[len - 1].pos_x && snake2[i].pos_y == snake1[len - 1].pos_y) {
        
            return 1;
        }
    }
// se choca la snake 2 con la snake 1
     for(int i = 0; i < len; i++) {
        if(snake1[i].pos_x == snake2[otherlen - 1].pos_x && snake1[i].pos_y == snake2[otherlen - 1].pos_y) {
        
            return 2;
        }
    }
    return 0;
}
int checkCollision(char players) {
    int toRet = checkSelfCollision(snake1);
    if(toRet == 1) {
        return toRet;
    }
    if(players == '2'){
    toRet =  checkSelfCollision(snake2);
    if(toRet == 1) {
        return 2;
    }
    toRet = checkOtherSnakeCollission();
    return toRet;
    }
    return 0;
}

int getLen(char string[]) {
    int i = 0;
    while(string[i] != '\0') {
        i++;
    }
    return i;
}

void endGame(char players, char winner) {
    char * p1 = "PLAYER ONE SCORE: ";
    char *p2;
    char *p3;
    char points1P[10];
    char points2P[10];
    intToStr(snake1->points, points1P);
    syscall(9, STDOUT); // Limpia la pantalla usando la llamada al sistema
    print(p1,18);
    print(points1P,getLen(points1P));
    nlPrint();
    if(players == '2') {
        p2 = "PLAYER TWO SCORE: ";
        p3 = "WINNER: PLAYER ";
        char win[2];
        win[0] = winner;
        win[1] = '\0';
        print(p2,18);
        intToStr(snake2->points, points2P);
        print(points2P,getLen(points2P));
        nlPrint();
        if(winner == '0') {
            p3 = "NO WINNER";
            print(p3,9);
        } else {
        print(p3, 15);
        print(win,1);
        }
    }
    sleep(25);
    syscall(9, STDOUT);
    syscall(7, STDOUT);
    return;
}   

char isValidKey1(char key){
    if(key=='w'|| key=='s'|| key=='a'|| key=='d'||key=='W'|| key=='S'|| key=='A'|| key=='D'||key=='x'||key=='X'){
        return 1;
    }
    return 0;
}

char isValidKey2(char key){
    if(key=='i'|| key=='k'|| key=='j'|| key=='l'||key=='I'|| key=='K'|| key=='J'|| key=='L'){
        return 1;
    }
    return 0;
}

void play(char players){
    syscall(6, STDOUT);
    resetGameState();
    snakeCanvas();
    iniSnake1();
    putSnake(snake1);
    putRandomCircle();
    char keyPressed;
    if(players == '2') {    
        iniSnake2();
        putSnake(snake2);
    }
    printPoints();
    char newKey;
    while(1){ 
        syscall(14,STDIN,&newKey);
        if (isValidKey1(newKey) || isValidKey2(newKey)){
            keyPressed = newKey;
        }
        if('x' == keyPressed){
            endGame(players, '0');
            return;
        }
        direcSnake(players, keyPressed);
        if(isSnakeinPos(circle, snake1)){
            pointEarned(snake1);
            printPoints();
        }
        if(players == '2' && isSnakeinPos(circle, snake2)){
            pointEarned(snake2);
            printPoints();
        }
        moveSnake(snake1);
        if(players == '2') {
            moveSnake(snake2);
        }
        int collision1 = checkCollision(players);
        if(collision1 == 1 || collision1 == 2) {
            syscall(11,1, STDOUT);
            endGame(players, collision1 == 1 ? '2' : '1');
            return;
        } else if(collision1 == 3){
            syscall(11,1, STDOUT);
            endGame(players, '0');
            return;
        }
        sleep(4);
    }
    return;
}

