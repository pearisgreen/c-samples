//hdr
#ifndef TCP_H
#define TCP_H

#include <netinet/in.h>

struct tcp_connection
{
    struct hostent *server;
    struct sockaddr_in server_addr;
    int port;
    int tcp_socket;
};

#include <openssl/ssl.h>

int tcp_connect(struct tcp_connection *tcp_conn, char *host, int port);

int tcp_send(struct tcp_connection *tcp_conn, char *request);

int tcp_recv(struct tcp_connection *tcp_conn, int (*callback)(char *data, int size));

int tcp_close(struct tcp_connection *tcp_conn);


#include <stdio.h>

//set this if you want to get logs
extern FILE *TCP_LOG;

#endif

//src
#ifndef TCP_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#ifndef TCPUTIL_C
#define TCPUTIL_C
#include "tcputil.c"
#undef TCPUTIL_C
#endif

FILE *TCP_LOG = 0;

int tcp_connect(struct tcp_connection *tcp_conn, char *host, int port)
{
    tcp_conn->tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    tcp_conn->port = port;

    if(tcp_conn->tcp_socket == -1)
    {
        printf("-----!error opening tcp_socket!-----\n");
        return 0;
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "+ tcp socket succesfully opened\n");

    if((tcp_conn->server = gethostbyname(host)) == 0)
    {
        close(tcp_conn->tcp_socket);
        printf("-----!error getting hostname!-----\n");
        return 0; 
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "%s -----------\n", tcp_conn->server->h_name);
    
    /*if(TCP_LOG)
    {
        for(int i = 0; tcp_conn->server->h_addr_list[i]; i++)
            fprintf(TCP_LOG, " -> %s\n", inet_ntoa(*(struct in_addr*)(tcp_conn->server->h_addr_list[i]))); 
    }
    */

    bzero(&tcp_conn->server_addr, sizeof(tcp_conn->server_addr));

    tcp_conn->server_addr.sin_family = AF_INET;

    bcopy(tcp_conn->server->h_addr, &tcp_conn->server_addr.sin_addr.s_addr, tcp_conn->server->h_length);

    tcp_conn->server_addr.sin_port = htons(tcp_conn->port);

    if(connect(tcp_conn->tcp_socket, &tcp_conn->server_addr, sizeof(tcp_conn->server_addr)) < 0)
    {
        printf("-----!error connection to server!-----\n");
        return 0;
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "succefully connected to server\n");

    return 1;
}

int tcp_send(struct tcp_connection *tcp_conn, char *request)
{
    if(TCP_LOG)
        fprintf(TCP_LOG, "request:\n%s\n", request);

    //send request 
    int sent_bytes = 0;
    int total_bytes = strlen(request);
    while(sent_bytes < total_bytes)
    {
        int bytes = write(tcp_conn->tcp_socket, request + sent_bytes, total_bytes - sent_bytes);
        if(bytes < 0)
        {
            printf("-----!error sending bytes: %i!-----\n", sent_bytes);
            return 0;
        }
        if(bytes == 0)
            break;

        sent_bytes += bytes;
    }

    if(TCP_LOG)
        fprintf(TCP_LOG, "succesfully sent request\n");

    return total_bytes;
}

int tcp_recv(struct tcp_connection *tcp_conn, int (*callback)(char *data, int size))
{
    int bytes = 0;
    int buffer_size = 1024;
    char buffer[buffer_size];
    do
    {
        bytes = recv(tcp_conn->tcp_socket, buffer, buffer_size, 0);

        if(bytes > 0)
        {
            buffer[bytes] = 0;
            if(callback(buffer, bytes) == -1)
                break;
        }

    } while(bytes > 0);
}

int tcp_close(struct tcp_connection *tcp_conn)
{
    close(tcp_conn->tcp_socket);

    return 1;
}

#endif
