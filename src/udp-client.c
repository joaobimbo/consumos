#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mcc_scan.h>

int main(int argc, char **argv){

  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  char *ip = "192.168.88.224";//"127.0.0.1";
  int port = atoi(argv[1]);

  int sockfd;
  struct sockaddr_in addr;
  char buffer[1024];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  bzero(buffer, 1024);
  strcpy(buffer, "Hello, World!!!!");
  sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr));
  printf("[+]Data send: %s\n", buffer);
  addr_size = sizeof(addr);

while(1){
  printf("A receber .....\n");

  struct DATA_SENT D;  

  bzero(buffer, 1024);
  recvfrom(sockfd, &D, sizeof(struct DATA_SENT), 0, (struct sockaddr*)&addr, &addr_size);
  printf("[+]Data recv: %f\n",D.voltage[0][0]);
  //write_to_file(D);
}
  return 0;
}