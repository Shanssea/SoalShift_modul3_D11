#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

//pertama buat thread dulu, buatnya 2 cukup untuk 2 thread
pthread_t tid[1];

//yang ini fungsi untuk buat directory, make system mkdir
void *makedir1(void *argv){
    system("mkdir -p /home/sea/Documents/Sisop/Modul_3/FolderProses1");
}
//ini juga sama, mkdir untuk folder proses 2
void *makedir2(void *argv){
    system("mkdir -p /home/sea/Documents/Sisop/Modul_3/FolderProses2");
}
//ini fungsi untuk mencatat list proses (ps -aux) maksimal 10 (head -n 11)--> untuk ngambil 11 baris dari ps -aux (termasuk headernya), (tail -n 10)--> untuk mengambil 10 baris terakhir. jadi headernya tidak tercatat di file nantinya. 
void *write1(void *argv){
    system("ps -aux | head -n 11 | tail -n 10 > /home/sea/Documents/Sisop/Modul_3/FolderProses1/SimpanProses1.txt");
}
//sama kayak yang atas
void *write2(void *argv){
    system("ps -aux | head -n 11 | tail -n 10 > /home/sea/Documents/Sisop/Modul_3/FolderProses2/SimpanProses2.txt");
}
//fungsi kompres, pertama masuk ke foldernya dulu (cd). lalu di kompres dengan menggunakan zip -q. -q berfungsi agar tidak tercetak info kompress seperti yang tertera di lampiran
void *compress1(void *argv){
    system("cd /home/sea/Documents/Sisop/Modul_3/FolderProses1/ && zip -q KompresProses1.zip SimpanProses1.txt && rm SimpanProses1.txt");
}
//sama seperti yang atas
void *compress2(void *argv){
    system("cd /home/sea/Documents/Sisop/Modul_3/FolderProses2/ && zip -q KompresProses2.zip SimpanProses2.txt && rm SimpanProses2.txt");
}
//fungsi extract. pertama masuk ke foldernya dahulu, lalu extract filenya menggunakan unzip -q. -q berfungsi agar tidak tercetak info kompress seperti yang tertera di lampiran
void *extract1(void *argv){
    system("cd /home/sea/Documents/Sisop/Modul_3/FolderProses1/ && unzip -q KompresProses1.zip");
}
// sama seperti yang atas
void *extract2(void *argv){
    system("cd /home/sea/Documents/Sisop/Modul_3/FolderProses2/ && unzip -q KompresProses2.zip");
}



int main(){
	//membuat thread untuk fungsi makedir1 dan makedir2 agar berjalan bersama-sama, maka di joinkan setelah kedua thread telah selesai dibuat.
	pthread_create(&tid[0], NULL, &makedir1, NULL);
	pthread_create(&tid[1], NULL, &makedir2, NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[0],NULL);
	
	//membuat thread dengan fungsi write1 dan write2
	pthread_create(&tid[0], NULL, &write1, NULL);
	pthread_create(&tid[1], NULL, &write2, NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	//membuat thread dengan fungsi compress1 dan compress2
	pthread_create(&tid[0], NULL, &compress1, NULL);
	pthread_create(&tid[1], NULL, &compress2, NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

	//mencetak "Menunggu 15 detik untuk mengekstrak kembali", lalu menggunakan sleep untuk menghentikan program selama 15 detik
	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    	sleep(15);	

	//membuat thread untuk menjalankan fungsi extract1 dan extract2
	pthread_create(&tid[0], NULL, &extract1, NULL);
	pthread_create(&tid[1], NULL, &extract2, NULL);
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);

}
