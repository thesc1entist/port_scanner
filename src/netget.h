#ifndef NETGET_H
#define NETGET_H

#include <stdio.h> // for size_t

typedef struct
{
    int fd;
    int is_done;
    size_t pos;
    size_t buflen;
    char buf[BUFSIZ];
} TEXTSCK;

char *netgets(char *str, size_t size, TEXTSCK *stream);
int netgetc(TEXTSCK *stream);
void textsckinit(TEXTSCK *stream, int fd);

#endif // NETGET_H
