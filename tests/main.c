#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>
#include "tlpi_hdr.h"

#define BUF_SIZE 10 // Maximum size of messages exchanged  between client and server

int main(int argc, char **argv)
{
        struct sockaddr_in addr;
        int socket_fd, j;
        short port;
        size_t msg_len;
        ssize_t num_bytes;
        char resp[BUF_SIZE];

        if (argc < 3 || strcmp(argv[1], "--help") == 0)
                usageErr("Usage: %s host port\n", argv[0]);

        port = getInt(argv[2], GN_ANY_BASE, argv[0]);
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1)
                errExit("socket");

        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0)
                fatal("inet_pton failed for address '%s'", argv[1]);

        return 0;
}
