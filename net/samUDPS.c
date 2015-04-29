/* Sample UDP server */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char**argv) {
   int sockfd = 0, n;
   struct sockaddr_in servaddr, cliaddr;
   socklen_t len;
   char mesg[1000];

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   inet_aton("192.168.0.11", &servaddr.sin_addr.s_addr);
   //servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(52888);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
      printf("recvfrom ----------------------------------------------\n");
      sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received the following:\n");
      printf("%s",mesg);
      printf("-------------------------------------------------------\n");
   }
}

