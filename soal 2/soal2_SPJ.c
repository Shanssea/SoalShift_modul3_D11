#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 8080
#include <sys/ipc.h>
#include <sys/shm.h>

int *stock;

void *cetak (){
	while (1){
		printf("Stock = %d\n",*stock);
		sleep(5);
	}
}
void *tambah (void *msg){
	char *m = (char *)msg;
	if (strcmp(m,"tambah")==0)(*stock)++;
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1; 
    int status = 0;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    pthread_t t,y;

    key_t key = 8787;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stock = (int *)shmat(shmid, NULL, 0);
	*stock = 50;
     
    //char *hello = "Hello from server";
    
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
	pthread_create(&t,NULL,cetak,NULL);
		//printf("1\n");
    while(1){
	    	
	    	valread = read( new_socket , buffer, 1024);
		pthread_create(&y,NULL,tambah,(void *)buffer);
		
    }
    return 0;
}


