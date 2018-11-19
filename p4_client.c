#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define DEST_IP "127.0.0.1"
#define DEST_PORT 8005
int main()
{
    int sockfd;
    int nBytes = 0;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in dest_addr;
    char buffer[1024] = {0};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(DEST_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);

    memset(&(dest_addr.sin_zero), '\0', 8);

    connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));

    int r = read(sockfd, buffer, 1024);
    printf("%s\n", buffer);
    memset(&(buffer), '\0', sizeof(buffer));

    while (1)
    {
        printf("Type a sentence to send to server:\n");
        scanf("%s", &buffer[0]);
        nBytes = strlen(buffer);
   
        send(sockfd, buffer, strlen(buffer), 0);
        printf("%s", buffer);
        memset(&(buffer), '\0', sizeof(buffer));
   
        nBytes = recv(sockfd, buffer, 1024, 0);
        printf("Received from server: %s\n", buffer);
        memset(&(buffer), '\0', sizeof(buffer));
    }
    return 0;
}