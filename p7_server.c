#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
void *connection_handler(void *);

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
    	printf("Could not create socket\n");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
    	printf("bind failed. Error\n");
    	return 1;
	}
	listen(socket_desc , 3);
	printf("Waiting for incoming connections...\n");
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
    	puts("Connection accepted\n");
    	pthread_t sniffer_thread;
    	new_sock = malloc(sizeof(*new_sock));
    	*new_sock = client_sock;
    	if(pthread_create(&sniffer_thread ,NULL, connection_handler,(void*) new_sock) < 0)
    	{
        	printf("could not create thread\n");
        	return 1;
    	}
	}
	if (client_sock < 0)
	{
    	printf("accept failed\n");
    	return 1;
	}
	return 0;
}
void *connection_handler(void *socket_desc)
{
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
    	write(sock , client_message , strlen(client_message));
    	printf("%s\n",client_message);
	}
	if(read_size == 0)
	{
    	printf("Client disconnected");
    	fflush(stdout);
	}
	else if(read_size == -1)
	{
    	printf("recv failed");
	}
	free(socket_desc);
	close(sock);
	pthread_exit(NULL);
	return;
}