#include "tlpi_hdr.h"
#include "inet_sockets.h"

#define MAX_PORT 65535

int main(int argc, char **argv)
{
        int sock_fd;

        if (argc < 3 || strcmp(argv[1], "--help") == 0)
                usageErr("Usage: %s host port\n", argv[0]);

        for (int i = 0; i < 100; i++)
        {
                sock_fd = inetConnect(argv[1], argv[2], SOCK_STREAM); // tcp
                if (sock_fd == -1)
                        continue;

                printf("port %d open\n", i);
        }

        return 0;
}
