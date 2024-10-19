#include <stdint.h>
#include <string.h>
#include <naiveConsole.h>

char toLetter(int i){

    switch(i){

        case 130:
        return '1';
        case 131:
        return '2';
        case 132:
        return '3';
        case 133:
        return '4';
        case 134:
        return '5';
         case 135:
        return '6';
        case 136:
        return '7';
        case 137:
        return '8';
        case 138:
        return '9';
        case 139:
        return '0';


        case 144:
        return 'q';
        case 145:
        return 'w';
        case 146:
        return 'e';
        case 147:
        return 'r';
        case 148:
        return 't';
         case 149:
        return 'y';
        case 150:
        return 'u';
        case 151:
        return 'i';
        case 152:
        return 'o';
        case 153:
        return 'p';

        case 158:
        return 'a';
        case 159:
        return 's';
        case 160:
        return 'd';
        case 161:
        return 'f';
        case 162:
        return 'g';
         case 163:
        return 'h';
        case 164:
        return 'j';
        case 165:
        return 'k';
        case 166:
        return 'l';

        case 172:
        return 'z';
        case 173:
        return 'x';
        case 174:
        return 'c';
        case 175:
        return 'v';
        case 176:
        return 'b';
         case 177:
        return 'n';
        case 178:
        return 'm';
        case 179:
        return ',';
        case 180:
        return '.';

        case 185:
        return ' ';

        case 156:
        ncNewline();
        return;

        default:
        return '-';
    }

}