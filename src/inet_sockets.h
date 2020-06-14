#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H

#include <sys/socket.h>
#include <netdb.h>

/**
 * The inetConnect() function creates a socket with the given socket type, and connects
 * it to the address specified by host and service. This function is designed for TCP or
 * UDP clients that need to connect their socket to a server socket.
 * 
 * @param host address to connect to
 * @param service TCP port
 * @return Returns a file descriptor on success, or –1 on error
 */
int inetConnect(const char *host, const char *service, int type);

/** 
 * The inetListen() function creates a listening stream ( SOCK_STREAM ) socket bound
 * to the wildcard IP address on the TCP port specified by service. This function is
 * designed for use by TCP servers.
 * 
 * @param service TCP port
 * @param backlog argument specifies the permitted backlog of pending connections
 * @param addrlen If addrlen is specified as a non- NULL pointer, then the location it points to is used
 * to return the size of the socket address structure corresponding to the returned file descriptor.
 * This value allows us to allocate a socket address buffer of the appropri-ate size 
 * to be passed to a later accept() call if we want to obtain the address of a connecting client.
 * @return Returns a file descriptor on success, or –1 on error
 */
int inetListen(const char *service, int backlog, socklen_t *addrlen);

/** 
 * The inetBind() function creates a socket of the given type, bound to the wildcard
 * IP address on the port specified by service and type. (The socket type indicates
 * whether this is a TCP or UDP service.) This function is designed (primarily) for
 * UDP servers and clients to create a socket bound to a specific address.
 * 
 * @param service TCP port
 * @param type socket type indicates wether this is a TCP or UDP service
 * @return Returns a file descriptor on success, or –1 on error
 */
int inetBind(const char *service, int type, socklen_t *addrlen);

/** 
 * The inetAddressStr() function converts an Internet socket address to printable form.
 * Given a socket address structure in addr, whose length is specified in addrlen,
 * inetAddressStr() returns a null-terminated string containing the corresponding host-
 * name and port number in the following form: (hostname, port-number)
 * 
 * @param addrStr the string is returned in the buffer pointed to by addrStr.
 * @param addrStrLen The caller must specify the size of the buffer in addrStrLen, If the returned string would exceed (addrStrLen – 1) bytes, it is truncated.
 * @return Returns pointer to addrStr, a string containing host and service name
 * 
 * NOTE: The constant IS_ADDR_STR_LEN defines a suggested size for the addrStr buffer that should be large enough to handle all possible return strings.
 */
char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen);

#define IS_ADDR_STR_LEN 4096 /* Suggested length for string buffer that caller should pass to inetAddressStr(). Must be greater than (NI_MAXHOST + NI_MAXSERV + 4) */

#endif // INET_SOCKETS_H
