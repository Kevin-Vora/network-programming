#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
    	printf("Could not create socket");
	}
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
    	printf("connect failed");
    	return 1;
	}
	puts("Connected\n");
	while(1)
	{
    	printf( "Type 'e' to end session:\n");
    	printf("type message:");
    	scanf("%s" , message);
    if( message[0] == 'e')
    	{
         	close(sock);
         	return 0;
    	}

    	else{
        	if( send(sock , message , strlen(message) , 0) < 0)
   		 {
       			 printf("Send failed");
       			 return 1;
   		 }
   		 if( recv(sock , server_reply , 2000 , 0) < 0)
   		 {
       			 printf("recv failed");
       			 break;
   		 }
    	printf("Server reply : %s", server_reply);
    	}
	}

	close(sock);
	return 0;
}