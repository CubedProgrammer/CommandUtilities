#include<arpa/inet.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
void get_ip(const char *host, char *cbuf)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;
    struct addrinfo *addresses;
    int res = getaddrinfo(host, NULL, &hints, &addresses);
    if(res != 0)
        printf("%d %s\n", errno, gai_strerror(res));
    struct sockaddr_in *addrin;
    for(struct addrinfo *n = addresses; n != NULL; n = n->ai_next)
    {
        addrin = (struct sockaddr_in *)n->ai_addr;
        strcpy(cbuf, inet_ntoa(addrin->sin_addr));
        cbuf += strlen(cbuf);
        *cbuf = '\n';
        ++cbuf;
    }
    *cbuf = '\0';
}
