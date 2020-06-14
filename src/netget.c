#include <stdlib.h>
#include <unistd.h>

#include "netget.h"

void textsckinit(TEXTSCK *stream, int fd)
{
    stream->fd = fd;
    stream->is_done = 0;
    stream->pos = 0;
    stream->buflen = 0;
}

int netgetc(TEXTSCK *stream)
{
    if (stream->is_done)
        return EOF;

    if (stream->pos == stream->buflen)
    {
        ssize_t nread = read(stream->fd, stream->buf, sizeof(stream->buf));
        if (nread <= 0)
            stream->is_done = 1;

        stream->pos = 0;
        stream->buflen = nread;
    }

    return stream->buf[stream->pos++];
}

char *netgets(char *str, size_t size, TEXTSCK *stream)
{
    char *s = str;
    int c = 0;

    if (stream->is_done)
        return NULL;

    for (size_t i = 0; i != size - 1 && c != '\n'; i++)
    {
        c = netgetc(stream);
        if (c == EOF)
            break;

        *s++ = c;
    }

    *s = '\0';
    return str;
}
