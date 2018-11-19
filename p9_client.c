#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h> 
#include <arpa/inet.h>  
#include <stdio.h>    
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>

#define MAX_LEN  1024   
#define MIN_PORT 1024  
#define MAX_PORT 65535  

int main(int argc, char *argv[]) {

  int sock;                  
  char send_str[MAX_LEN];  
  struct sockaddr_in mc_addr;
  unsigned int send_len;  
  char* mc_addr_str;       
  unsigned short mc_port;
  unsigned char mc_ttl=1;
  if (argc != 3) {
    fprintf(stderr, 
            "Usage: %s <Multicast IP> <Multicast Port>\n", 
            argv[0]);
    exit(1);
  }

  mc_addr_str = argv[1];    
  mc_port     = atoi(argv[2]); 
  if ((mc_port < MIN_PORT) || (mc_port > MAX_PORT)) {
    fprintf(stderr, "Invalid port number argument %d.\n",
            mc_port);
    fprintf(stderr, "Valid range is between %d and %d.\n",
            MIN_PORT, MAX_PORT);
    exit(1);
  }
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket() failed");
    exit(1);
  }
  
  if ((setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, 
       (void*) &mc_ttl, sizeof(mc_ttl))) < 0) {
    perror("setsockopt() failed");
    exit(1);
  } 
  
  memset(&mc_addr, 0, sizeof(mc_addr));
  mc_addr.sin_family      = AF_INET;
  mc_addr.sin_addr.s_addr = inet_addr(mc_addr_str);
  mc_addr.sin_port        = htons(mc_port);

  printf("Begin typing (return to send, ctrl-C to quit):\n");
  memset(send_str, 0, sizeof(send_str));

  while (fgets(send_str, MAX_LEN, stdin)) {
    send_len = strlen(send_str);
    if ((sendto(sock, send_str, send_len, 0, 
         (struct sockaddr *) &mc_addr, 
         sizeof(mc_addr))) != send_len) {
      perror("sendto() sent incorrect number of bytes");
      exit(1);
    }
    memset(send_str, 0, sizeof(send_str));
  }

  close(sock);  

  exit(0);
}