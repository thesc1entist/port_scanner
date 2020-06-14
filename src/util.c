#include "util.h"
#include <string.h>

char *ChompWS(char *str)
{
    str += strspn(str, " \t\r\n");

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' ||
                         *end == '\r' || *end == '\n'))
        end--;

    end[1] = '\0';
    return str;
}
