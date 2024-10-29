typedef struct {
    char aux;       //sin este registro cunado queiro rempalzar la hora en la syscall por alguna razon se pisa el buffer del shell
    char hours;
    char mins;
    char seconds;
}time;

void getTime();
void printTime();