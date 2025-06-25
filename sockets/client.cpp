/*
    Client Code
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

int main() {
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET is IPv4, SOCK_STREAM is TCP
    
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);        // port; convert endian
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1; localhost

    // Connect to the port + address of the server 
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv) { die("connect()"); }    

    char msg[] = "hello";
    write(fd, msg, strlen(msg));

    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        die("read");
    }
    
    std::cout << "server says: " << rbuf << std::endl;
    close(fd);

    return 0;
}
