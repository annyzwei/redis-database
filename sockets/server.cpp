/*
    Server Code
    Uses IPv4+TCP 
    From: https://build-your-own.org/redis/03_hello_cs
*/ 

#include <sys/types.h>      // For data types like `socklen_t`
#include <sys/socket.h>     // For socket(), bind(), struct sockaddr, etc.
#include <netinet/in.h>     // For sockaddr_in, htons(), htonl(), AF_INET
#include <arpa/inet.h>      // For inet_pton(), inet_ntoa(), etc. (optional)
#include <unistd.h>         // For close()
#include <string.h>         // For memset()
#include <stdio.h>          // For perror(), printf(), etc.
#include <stdlib.h>         // For exit(), etc.
#include <iostream>

void die(const char *msg) {
    perror(msg);    
    exit(1);        
}

static void process_client_message(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        std::cerr << "read() error";
        return;
    }
    std::cout << "client says: " << rbuf << std::endl;

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET is IPv4, SOCK_STREAM is TCP
    
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // Allow reuse of address

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);        // port; convert endian
    addr.sin_addr.s_addr = htonl(0);    // wildcard IP 0.0.0.0
                                        // Means that it accepts connections on any of the IPs of this computer 

    // Binds to an address
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) { die("bind()"); }    

    // Create the socket
    rv = listen(fd, SOMAXCONN);
    if (rv) { die("listen()"); }

    while (true) {
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        if (connfd < 0) {
            continue;   // error
        }

        process_client_message(connfd);
        close(connfd);
    }

    return 0;
}
