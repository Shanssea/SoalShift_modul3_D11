#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock1 = 0, sock2 = 0, valread;
    struct sockaddr_in serv_addr;
    //char *msg;
    char buffer[1024] = {0};

    if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    if ((sock2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
     if (connect(sock2, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

	while (1){
		char inf[255];
		fgets(inf,255,stdin);
		int t;
		inf[strcspn(inf,"\n")] =0;
		
		send(sock1 , inf , strlen(inf) , 0 );
		send(sock2 , inf , strlen(inf) , 0 );
    //printf("Hello message sent\n");
		//valread = read( sock , buffer, 1024);
		//printf("%s\n",buffer );
	}
    return 0;
}
