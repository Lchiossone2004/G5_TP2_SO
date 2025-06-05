#include <shell.h>

static char buffer[WORD_BUFFER_SIZE] = {0};
static char letra[1] = {0};
static int ultimaLetra;
static int index = 0;


void shell()
{
    print(NEW_LINE);
    Command aux;
    while (1)
    {
        if (index == WORD_BUFFER_SIZE - 1)
        {
            chekCommand(aux);
            print(NEW_LINE);
            index = 0;
            ultimaLetra = 0;
        }
        getKey(letra, &index);
        if (*letra == 0 && index > 0)
        {
            index -= 1;
            buffer[index] = 0;
        }
        if (*letra == 1)
        {
            chekCommand(aux);
            print(NEW_LINE);
            index = 0;
            ultimaLetra = 0;
        }
        if (*letra != 0 && *letra != 1)
        {
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

void getKey()
{
    syscall(2, STDIN, letra, index);
}

void chekCommand(Command aux)
{
    deleteSpaces(buffer);
    aux = parseCommand(buffer);
    int command = processCommand(aux.command);
    if(command >= 0 && command <NUMBER_OF_COMMANDS){
        if (strchr(aux.command, '|')) {  
            pipeCommand(aux.arg_count, aux.args, aux.command);  
        }
        if(aux.arg_count == 1 && strCompare(aux.args[0],"-info")){
            commandInfo(command - 1, -1);
            nlPrint();
        }
        else{
        shell_table[command](aux.arg_count, aux.args, aux.command);
        }
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


    i = 0;
    while (index < len && input[index] != ' ' && i < 123) {
        toRet.command[i++] = input[index++];
    }
    toRet.command[i] = '\0';


    while (index < len && toRet.arg_count < MAX_ARGS) {

        while (index < len && input[index] == ' ') index++;
        if (index >= len) break;

        toRet.args[toRet.arg_count] = (char *)usr_malloc(sizeof(char) * MAX_ARG_LEN);
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
