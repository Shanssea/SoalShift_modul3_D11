
# Soal Shift Modul 3


## Daftar Soal
- [Soal 1](#soal-1)
- [Soal 2](#soal-2)
- [Soal 3](#soal-3)
- [Soal 4](#soal-4)
- [Soal 5](#soal-5)


## Soal 1

1. Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
	./faktorial 5 3 4
	3! = 6
	4! = 24
	5! = 120


```c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

int count=0;
pthread_t thread[255];

void *fact(void *arguments){
	int num = *((int *)arguments);
			long long int i, fact = 1;
			for ( i=1; i <= num; i++) fact = fact * i;
			printf("%d! = %lld\n",num,fact);
}

int main (int argc, char* argv[]){
	int arr[225];
	char c;
	int a;
	for (int i=0; c!='\n'; i++)
	{
		scanf("%d%c",&arr[i],&c);
		count++;
	}

	for (int i=0; i < count; i++)
	{
		for (int j = i+1; j < count; j++)
		{
			if(arr[i] > arr[j])
			{
				a = arr[i];
				arr[i] = arr[j];
				arr[j] = a;
			}
		}
	}

	
	int ans[255];
	int j = 0;
	for (int i = 0; i < count; i++)
	{			
		ans[i]= pthread_create( &(thread[i]), NULL, fact, &arr[i]);
		ans[i]= pthread_join(thread[i],NULL);
	}
	
	return 0;
}
```


Program diatas menggunakan thread untuk menghitung faktorial tiap input. Cara memasukkan inputnya adalah dengan menggunakan **(int argc, char* argv[])**, lalu di loop dimasukkan ke array arr[], di convert menggunakan atoi. Dengan menggunakan array, inputan dimasukkan ke array dan diurutkan. Lalu, looping sebanyak jumlah input, untuk membuat thread yang menjalankan fungsi fact dengan parameter arr[i], lalu thread di joinkan. Sehingga hasilnya berurutan.

## Soal 2

2.  Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
    
    1.  Terdapat 2 server: server penjual dan server pembeli
        
    2.  1 server hanya bisa terkoneksi dengan 1 client
        
    3.  Server penjual dan server pembeli memiliki stok barang yang selalu sama
        
    4.  Client yang terkoneksi ke server penjual hanya bisa menambah stok
        
        -   Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
            
    5.  Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
        
        -   Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
            
    6.  Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
        
        -   Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
            
        -   Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
            
    7.  Server penjual akan mencetak stok saat ini setiap 5 detik sekali
        
    8.  **Menggunakan thread, socket, shared memory**

*Server Penjual*
```c
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


```
Di Server Penjual, thread pertama berjalan untuk menjalankan fungsi cetak yang berisi mencetak jumlah Stock setiap 5 detik, lalu dengan looping while(true), server menerima input dari client dan disimpan di buffer, lalu membuat thread untuk menjalankan fungsi tambah, yang isinya menambah nilai Stock jika buffernya adalah "tambah".
 
*Server Pembeli*
```c
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
		if (*stock > 0){
			(*stock) --;
			printf("transaksi berhasil\n");
		}else{
			printf("transaksi gagal\n");
		}
}


int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1; 
    int status = 0;
    int addrlen = sizeof(address);
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
		if (strcmp(buffer,"beli")==0){
			pthread_create(&y,NULL,beli,NULL);
			pthread_join(y,NULL);			
		}
    }
    return 0;
}

```
Di Server Pembeli, dengan looping while(true), input dari client di cek, jika input = "beli" maka akan dibuat thread yang memanggil fungsi beli yang mengecek apakah Stock masih ada apa tidak. Jika stock tidak ada, maka tidak ada pengurangan pada Stock dan akan tercetak "transaksi gagal".  Jika Stock ada maka ada pengurangan Stock dan akan tercetak "transaksi berhasil". Lalu thread y di joinkan. 
Server Pembeli dan Server Penjual berbagi variable Stock dengan menggunakan Shared Memmory dengan syntax
```c
key_t key =  8787;  int shmid =  shmget(key,  sizeof(int), IPC_CREAT |  0666); stock =  shmat(shmid,  NULL,  0);
```

*Client*
```c
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
```
Pada Client dibuat 2 connection dengan 2 socket. 1 ke Server Pembeli dan 1 ke Server Penjual.

## Soal 3
3.  Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
    
    -  Terdapat 2 karakter Agmal dan Iraj
        
    -  Kedua karakter memiliki status yang unik
        
        -   Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
            
        -   Iraj memiliki Spirit_Status,  di awal program memiliki status 100
            
      -   Terdapat 3 Fitur utama
            

			-   All Status, yaitu menampilkan status kedua sahabat
    Ex: Agmal WakeUp_Status = 75
Iraj Spirit_Status = 30

			-   “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
    
			-   “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
    

		-   Terdapat Kasus yang unik dimana:
    
		    -   Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
        
		    -   Jika Fitur “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
        
		-   Program akan berhenti jika Salah Satu :
    
		    -   WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
        
		    -   Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)
        

  

  

	-   **Syarat Menggunakan Lebih dari 1 Thread**
```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int WakeUp_Status = 0;
int Spirit_Status = 100;
pthread_t th[3];
int counta=1, counti=1;
int c=10, d=10;
int A=0,I=0;


void setTimeout(int milliseconds)
{
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}

void *func (void *fitur)
{
	char *ft = (char *) fitur;
	//printf("input: %s\n",ft);
	pthread_t id = pthread_self();
	
	if (pthread_equal(id,th[0]))
	{
		printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n\n",WakeUp_Status,Spirit_Status);
	}
	else if (pthread_equal(id,th[1]))
	{
		if (counta == 3){
			printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
			I=1;
			
		}
		if (A == 1){
			c = 10;
			while (c > 0){
				setTimeout(1000);
				counti = 0;
				c--;
			}
			if (c==0) A=0;
		}
		else if (A == 0){
			WakeUp_Status+=15;
				counta++;		
		}
	}
	else if(pthread_equal(id,th[2]))
	{
		
		if (counti == 3){
			printf("Agmal Ayo Bangun disabled 10 s\n");
			A=1;
		}
		if (I == 1){
			d = 10;
			while (d > 0) {
				setTimeout(1000);
				counta = 0;
				d--;
			}
			if (d==0) I=0;
		}
		else if (I == 0){
			Spirit_Status-=20;
			counti++;
		}
		I = 0;
	}
	return NULL;
}



int main(){
	while (1){
		if (WakeUp_Status >= 100){
			printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
			break;
		}
		else if (Spirit_Status <= 0){
			printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
			break;
		}
		char inf[255];
		fgets(inf,255,stdin);
		int t;
		inf[strcspn(inf,"\n")] =0;
		
		if (strcmp(inf,"All Status")==0)
		{
			t=pthread_create(&(th[0]),NULL,func,(void*)inf);
			//t=pthread_join(th[0],NULL);
		}
		else if (strcmp(inf,"Agmal Ayo Bangun")==0)
		{
			t=pthread_create(&(th[1]),NULL,func,(void*)inf);
			//t=pthread_join(th[1],NULL);
		}
	
		else if (strcmp(inf,"Iraj Ayo Tidur")==0)
		{
			t=pthread_create(&(th[2]),NULL,func,(void*)inf);
			//t=pthread_join(th[2],NULL);	
		
		}

	}

	return 0;
}
```

Di program ini, jika input adalah "All Status" maka akan membuat thread th[0] , jika input adalah "Agmal Ayo Bangun" maka akan membuat thread th[1] dan jika input adalah "Iraj Ayo Tidur" maka akan membuat thread th[2]. Tiap thread menjalankan fungsi yang sama yaitu fungsi func, yang memiliki beberapa kondisi. Di fungsi func, thread akan menjalankan fungsinya sesuai dengan id nya.
Untuk fungsi disable fitur, dibuat semacam sinyal. Jika count > 3, maka sinyal akan berubah menjadi 1. Jika sinyal == 1, maka fungsi setTimeout berjalan selama 10 detik, jika sudah 10 detik, maka sinyal akan berubah menjadi 0 kembali.
Fungsi setTimeout berfungsi sebagai timer di program ini.  


## Soal 4


4.  Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip
    

**Dengan Syarat :**

-   Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
    
-   Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
    
-   Ketika Mengekstrak file .zip juga harus secara bersama-sama
    
-   Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
    
-   Wajib Menggunakan Multithreading
    
-   Boleh menggunakan system

*belum mengerjakan*
  
## Soal 5


5.  Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta.
    
    1.  Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat memberi nama pada monsternya.
        
    2.  Monster pemain memiliki _hunger status_ yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika _hunger status_ mencapai angka nol, pemain akan kalah. _Hunger status_ dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.
        
    3.  Monster pemain memiliki _hygiene status_ yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika _hygiene status_ mencapai angka nol, pemain akan kalah. _Hygiene status_' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).
        
    4.  Monster pemain memiliki _health status_ yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
        
    5.  Monster pemain dapat memasuki keadaan _battle_. Dalam keadaan ini, _food status_(fitur b), _hygiene status_'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. _Health status_ dari monster dimulai dari darah saat monster pemain memasuki _battle_. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian.
        
    6.  Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
        
        -   Pembeli (terintegrasi dengan game)
            
            -   Dapat mengecek stok makanan yang ada di toko.
                
            -   Jika stok ada, pembeli dapat membeli makanan.
                
        -   Penjual (terpisah)
            
            -   Bisa mengecek stok makanan yang ada di toko
                
            -   Penjual dapat menambah stok makanan.
                

Spesifikasi program:

1.  Program mampu mendeteksi input berupa _key press_. (Program bisa berjalan tanpa perlu menekan tombol _enter_)
    
2.  Program terdiri dari 3 _scene_ yaitu _standby_, _battle_, dan _shop_.
    
3.  Pada saat berada di _standby scene_, program selalu menampilkan health status, _hunger status,_ _hygiene status_, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, _battle_, _shop_, dan exit. Contoh :
    

  

		Standby Mode
		Health : [health status]
		Hunger : [hunger status]
		Hygiene : [hygiene status]
		Food left : [your food stock]
		Bath will be ready in [cooldown]s
		
		Choices
		1.  Eat
		2.  Bath
		3.  Battle
		4.  Shop
		5.  Exit
    

  

4.  Pada saat berada di _battle scene_, program selalu menampilkan _health status_ milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :
    

  

		Battle Mode
		Monster’s Health : [health status]
		Enemy’s Health : [enemy health status]

		Choices
		1.  Attack
		2.  Run
    

  

5.  Pada saat berada di _shop scene_ versi pembeli, program selalu menampilkan _food stock_ toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke _standby scene_. Contoh :
    

  

		Shop Mode
		Shop food stock : [shop food stock]
		Your food stock : [your food stock]

		Choices
		1.  Buy
		2.  Back
    

  

6.  Pada program penjual, program selalu menampilkan _food stock_ toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :
    

  

		Shop
		Food stock : [shop food stock]

		Choices
		1.  Restock
		2.  Exit
    

  

7.  Pastikan terminal hanya mendisplay status **detik ini** sesuai scene terkait (hint: menggunakan system(“clear”))

*belum mengerjakan*
