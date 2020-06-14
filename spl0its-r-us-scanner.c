/**
 * multi threaded port scanner
 * the-scientist@rootstorm.com
 * spl0its-r-us security
 * 
 * $ gcc spl0its-r-us-scanner.c -lpthread -o spl0its-r-us-scanner
 * $ ./spl0its-r-us-scanner 127.0.0.1 0:6000
 * 
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define NUM_THREADS (20)

#define handle_error_en(en, msg)    \
        do                          \
        {                           \
                errno = en;         \
                perror(msg);        \
                exit(EXIT_FAILURE); \
        } while (0)

#define handle_error(msg)           \
        do                          \
        {                           \
                perror(msg);        \
                exit(EXIT_FAILURE); \
        } while (0)

struct thread_info
{
        pthread_t thread_id;
        char *host;
        long start_port;
        long end_port;
};

static void *
scanner(void *arg)
{
        struct thread_info *tinfo = arg;
        struct sockaddr_in addr;

        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        if (inet_pton(AF_INET, tinfo->host, &addr.sin_addr) <= 0)
                handle_error("inet_pton failed");

        struct timeval timeout;
        int socket_fd;
        for (int port = tinfo->start_port; port <= tinfo->end_port; port++)
        {
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;

                socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (socket_fd == -1)
                        handle_error("socket");

                if (setsockopt(socket_fd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&timeout, sizeof(struct timeval)) < 0)
                {
                        close(socket_fd);
                        handle_error("setsockopt failed");
                }

                addr.sin_port = htons(port);
                if (connect(socket_fd, (struct sockaddr *)&addr, sizeof addr) != -1)
                {
                        printf("port %d open\n", port);
                        close(socket_fd);
                }
                close(socket_fd);
        }

        return NULL;
}

int main(int argc, char **argv)
{
        if (argc < 3)
        {
                fprintf(stderr, "Usage: %s host start_port:end_port\n", argv[0]);
                return 1;
        }

        long start_port = 0;
        long end_port = 0;
        char *endptr = NULL;
        char *range = argv[2];

        start_port = strtol(range, &endptr, 10);
        if (*endptr == ':')
        {
                end_port = strtol(endptr + 1, &endptr, 10);
                if (*endptr != '\0')
                {
                        fprintf(stderr, "Usage: %s host start_port:end_port\n", argv[0]);
                        return 1;
                }
        }
        else if (*endptr == '\0')
                end_port = start_port;
        else
        {
                fprintf(stderr, "Usage: %s host start_port:end_port\n", argv[0]);
                return 1;
        }

        int total_ports = end_port - start_port;
        int extra_port = total_ports % NUM_THREADS;
        int ports_per_thread = total_ports / NUM_THREADS;
        struct thread_info tinfo[NUM_THREADS];

        int num_threads = ports_per_thread == 0 ? 1 : 20;
        for (int tnum = 0; tnum < num_threads; tnum++)
        {
                tinfo[tnum].host = argv[1];
                tinfo[tnum].start_port = start_port;
                start_port += (ports_per_thread - 1);

                if (tnum == num_threads - 1)
                        start_port += extra_port + 1;
                tinfo[tnum].end_port = start_port;
                start_port += 1;

                int s = pthread_create(&tinfo[tnum].thread_id, NULL,
                                       &scanner, &tinfo[tnum]);
                if (s != 0)
                        handle_error_en(s, "pthread_create");
        }

        for (int tnum = 0; tnum < num_threads; tnum++)
        {
                int s = pthread_join(tinfo[tnum].thread_id, NULL);
                if (s != 0)
                        handle_error_en(s, "pthread_join");
        }

        exit(EXIT_SUCCESS);

        return 0;
}
