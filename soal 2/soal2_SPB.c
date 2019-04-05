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
int status = 0;

void *beli (){
	//int m = *((int *)msg);
	//while(valread =  read( m , buffer, 1024)){
		if (status == 0)(*stock) --;
	//}
}
void *cekstock (){
	if (*stock <= 0){
		printf("transaksi gagal\n");
		status = 1;
	}
	else printf("transaksi berhasil\n");
}


int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1; 
    int status = 0;
    int addrlen = sizeof(address);
    //char buffer[1024] = {0};
    pthread_t t,y;
    

	key_t key = 8787;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stock = shmat(shmid, NULL, 0);
	*stock = 10;
    
      
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
	
    while(1){
	    	char buffer[1024] = {0};
	    	valread = read( new_socket , buffer, 1024);
		//printf("status 1 = %d\n",status);
		if (strcmp(buffer,"beli")==0){
			pthread_create(&t,NULL,cekstock,NULL);
			pthread_create(&y,NULL,beli,NULL);
			pthread_join(y,NULL);			
			status = 0;
		}
		//printf("%s = %d\n",buffer,*stock);
		
    }
    return 0;
}




