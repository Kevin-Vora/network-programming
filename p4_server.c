#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define MYPORT 8005
#define BACKLOG 5
int main()
{
	int sockfd, new_fd;
	int n;
	pid_t pid;
	char msg[1024] = {0};

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("Could not create socket!!!\n");
	}
	printf("parent id:%d\n ", getpid());

	struct sockaddr_in myaddr, dest_addr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(MYPORT);
	myaddr.sin_addr.s_addr = INADDR_ANY;

	memset(&(myaddr.sin_zero), '\0', 8);

	int b = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr));
	if (b < 0)
		printf("Bind failed!\n");
	
	int list = listen(sockfd, BACKLOG);
	if (list == -1)
		printf("Server didnt start!\n");
	int sin_size = sizeof(struct sockaddr_in);
	while (1)
	{
		new_fd = accept(sockfd, (struct sockaddr *)&dest_addr, &sin_size);
		if (new_fd < 0)
			exit(1);
		printf("connected to client: %s\n", inet_ntoa(dest_addr.sin_addr));
		if ((pid = fork()) == 0)
		{
			printf("in process %d\n ", getpid());
			close(sockfd);
			char *msg3 = "Welcome! You are connected to the server!\n";
			send(new_fd, msg3, strlen(msg3), 0);
			//memset(&(msg),'\0',sizeof(msg));
			while (1)
			{
				n = recv(new_fd, msg, 1024, 0);
				if (n < 0)
				{
					close(new_fd);
					break;
				}
				printf("The client sent %s\n", msg);
				char *msg2 = "Back at you!\n";
				send(new_fd, msg2, strlen(msg2), 0);
				memset(&(msg), '\0', sizeof(msg));
			}
		}
		else
		{
			//printf("i an bald!\n");
		}
	}
	close(new_fd);
	return 0;
}