#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int hostname_to_ip(char *host, char *ip);

int print_hex(char *data, int size);

int main()
{
    int p_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in p_addr;

    p_addr.sin_family = AF_INET;
    p_addr.sin_port = htons(9050);
    inet_pton(AF_INET, "127.0.0.1", &(p_addr.sin_addr.s_addr));

    //connect to socket
    if(connect(p_socket, &p_addr, sizeof(p_addr)) == -1)
        printf("error connecting to socket\n");
    else
        printf("succesfully connected to localhost\n");

    //sending data over tor
    struct sockaddr_in dest_addr;
    char ip_addr[256];
    memset(ip_addr, 0 , 256);
    if(hostname_to_ip("www.reddit.com", ip_addr))
        printf("ip address: %s\n", ip_addr);
    else
        printf("error getting ip address\n");

    dest_addr.sin_addr.s_addr = inet_addr(ip_addr);
    int dest_port = 80;

    char buffer[256];
    memset(buffer, 0, 256);
    char *ptr = buffer;
    *(ptr++) = 5;
    *(ptr++) = 1;
    *(ptr++) = 0x00;

    if(send(p_socket, buffer, ptr-buffer, 0) == -1)
        printf("error sending to socket\n");

    if(recv(p_socket, buffer, 2, 0) == -1)
        printf("erro receving from socket\n");

    if(buffer[0] != 5 || buffer[1] == 0xFF)        
        printf("error\n");

    ptr = buffer;
    *(ptr++) = 5;
    *(ptr++) = 1;
    *(ptr++) = 0;
    *(ptr++) = 1;

    //HERE IS MY PROBLEM!!!
    memcpy(ptr, &dest_addr.sin_addr.s_addr, sizeof(dest_addr.sin_addr));
    ptr += sizeof(dest_addr.sin_addr);

    *(ptr++) = dest_port >> 8;
    *(ptr++) = dest_port & 0xFF;
    send(p_socket, buffer, ptr - buffer, 0);
    recv(p_socket, buffer, 4, 0);

    printf("socket ret: %i\n", (int)buffer[1]);

    if(buffer[1] != 0x00)
        printf("error\n");

    ptr = buffer + 4;
    if(buffer[3] == 1)
    {
        recv(p_socket, ptr, 1, 0);
    }
    else if(buffer[3] == 3)
    {
        recv(p_socket, ptr, 1, 0);
        recv(p_socket, ptr+1, *(unsigned char *)ptr + 2, 0);
    }
    else if(buffer[3] == 4)
    {
        recv(p_socket, ptr, 16 + 2, 0);
    }

    printf("success!\n");

    char req[] = "GET / HTTP/1.1\r\nHost: www.reddit.com\r\nConnection: close\r\n";

    send(p_socket, req, sizeof(req), 0);

    char ret_buffer[1024];
    memset(ret_buffer, 0, 1024);

    int bytes = recv(p_socket, ret_buffer, sizeof(ret_buffer), 0);
    printf("received bytes: %i\n", bytes);
    print_hex(ret_buffer, bytes);

    printf("response:\n%s\n", ret_buffer);

    close(p_socket);
}

int hostname_to_ip(char *host, char *ip)
{
    /* OBSOLETE
    struct hostent *hoste;

    if((hoste = gethostbyname(host)) == 0)
    {
        printf("error getting hostname\n");
        return 0;
    }
    else
        printf("successfully retreived hostname of: %s -> %s\n", host, hoste->h_name);

    for(int i = 0; hoste->h_addr_list[i] != 0; i++)
    {
        printf("here\n");
        printf("inet_ntoa: %s\n", inet_ntoa(*(struct in_addr*)(hoste->h_addr_list[i])));
        strcpy(ip, inet_ntoa(*(struct in_addr*)(hoste->h_addr_list[i])));
        printf("here\n");
        return 1;
    }
    */

    struct addrinfo hints, *infoptr;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;

    int result = getaddrinfo(host, NULL, &hints, &infoptr);
    if(result)
    {
        printf("error retreiving addrinfo of: %s\nerror: %s\n", host, gai_strerror(result));
        return 0;
    }

    struct addrinfo *ptr;

    for(ptr = infoptr; ptr != 0; ptr = ptr->ai_next)
    {
        getnameinfo(ptr->ai_addr, ptr->ai_addrlen, ip, 256, 0, 0, NI_NUMERICHOST);
        freeaddrinfo(infoptr);
        printf("host: %s\n", ip);

        return 1;
    }

    return 0; 
}

int print_hex(char *data, int size)
{
    for(int i = 0; i < size; i++)
        printf("[%i]: %02x\n",i , data[i]);
}
