#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    char domain_name[100];
    printf("Enter the domain name: ");
    scanf("%s", domain_name);

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(domain_name, NULL, &hints, &res)) != 0) {
        printf("getaddrinfo failed: %s\n", gai_strerror(status));
        return 1;
    }

    printf("IP addresses for %s:\n", domain_name);
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char ipstr[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s\n", ipstr);
    }

    freeaddrinfo(res);
    return 0;
}
