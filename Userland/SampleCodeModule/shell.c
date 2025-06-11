#include <shell.h>

static char buffer[WORD_BUFFER_SIZE] = {0};
static char letra[1] = {0};
static int ultimaLetra;
static int index = 0;

int getKey()
{
    return syscall(3, STDIN, letra, 1);
}
void shell()
{
    print(NEW_LINE);
    Command aux;
    int readed = -1;
    while (1)
    {
        if (index == WORD_BUFFER_SIZE - 1)
        {
            chekCommand(aux);
            print(NEW_LINE);
            index = 0;
            ultimaLetra = 0;
        }
        while (readed == -1){
            readed = getKey();
        }
        readed = -1;
        if (*letra == '\1' && index > 0)

        {
            print("\1");
            index -= 1;
            buffer[index] = 0;
        }
        else if (*letra == '\n')
        {
            print("\n");
            buffer[index++] = '\0';
            chekCommand(aux);
            print(NEW_LINE);
            index = 0;
            ultimaLetra = 0;
        }
        else
        {
            print(letra);
            if (*letra == ' ')
            {
                buffer[index++] = ' ';
            }
            else
            {
                buffer[index++] = *letra;
                ultimaLetra = index;
            }
            buffer[index] = 0;
        }
    }
}

int newComand(uint64_t argc,char *argv[]){
    char * command;
    int auxArgc = argc-1;
    command = argv[0];
    int is_foreground = 1;
    if(strCompare(argv[0],"&")){
        is_foreground = 0;
        auxArgc--;
    }
    char * argv1[auxArgc];
    for(int i = auxArgc; i < argc; i++){
        argv1[i] = argv[i];
    }
    int commandNum = processCommand(command);
    return shell_table[commandNum](auxArgc,argv1, commandNum,is_foreground);
}

void chekCommand(Command aux)
{
    aux = parseCommand(buffer);
    int pipe_pos = 0;
    for(int i = 0; i<aux.arg_count; i++){
        if(strCompare(aux.args[i],"|")){
            pipe_pos = i;
        }
    }
    if(pipe_pos == 0){
        newComand(aux.arg_count,aux.args);
    }
    else{
        int new_pipe[2];
        usr_open_pipe(&new_pipe[0], &new_pipe[1]);
        int pid1 = newComand(pipe_pos,aux.args);
        pipe_pos++;
        int pid2 = newComand(aux.arg_count - pipe_pos,aux.args + pipe_pos);
        usr_change_std(pid1,STDOUT, new_pipe[1]);
        usr_change_std(pid2,STDIN, new_pipe[0]);
        usr_close_pipe(new_pipe[0]);
        usr_close_pipe(new_pipe[1]);
    }

    clearBuffer();
    index = 0;
    freeCommand(&aux);
}

int processCommand(char *input) {
    if (index == 0 && input[0] == 0) {
        return -1;
    }

    for (int j = 0; j < NUMBER_OF_COMMANDS; j++) {
        if (strCompare(commands[j], input)) {
            return j + 1; 
        }
    }
    char commandPart[WORD_BUFFER_SIZE] = {0};
    int i = 0;
    while (input[i] != ' ' && input[i] != 0 && i < WORD_BUFFER_SIZE - 1) {
        commandPart[i] = input[i];
        i++;
    }
    commandPart[i] = 0;

  
    for (int j = 0; j < NUMBER_OF_COMMANDS; j++) {
        if (strCompare(commands[j], commandPart)) {
            return j + 1;
        }
    }

    return 0;
}

void clearBuffer(){
    for(int i = 0; i < index ; i++){
        buffer[i] = ' ';
    }
}

Command parseCommand(char *input) {
    Command toRet;
    int len = strSize(input);
    int i = 0;
    int index = 0;

    toRet.args = (char **)usr_malloc(sizeof(char *) * MAX_ARGS);
    toRet.arg_count = 0;


    while (index < len && input[index] == ' ') index++;

    while (index < len && toRet.arg_count < MAX_ARGS) {

        while (index < len && input[index] == ' ') index++;
        if (index >= len) break;

        char * aux = (char *)usr_malloc(sizeof(char) * MAX_ARG_LEN);
        toRet.args[toRet.arg_count] = aux;
        i = 0;

        while (index < len && input[index] != ' ' && i < MAX_ARG_LEN - 1) {
            toRet.args[toRet.arg_count][i++] = input[index++];
        }
        toRet.args[toRet.arg_count][i] = '\0';
        toRet.arg_count++;
    }

    return toRet;
}


void freeCommand(Command *cmd) {
    for (int i = 0; i < cmd->arg_count; i++) {
        usr_free(cmd->args[i]);
    }
    usr_free(cmd->args);
}
