# Soal Shift Modul 3


## Daftar Soal
- [Soal 1](#soal-1)
- [Soal 2](#soal-2)
- [Soal 3](#soal-3)
- [Soal 4](#soal-4)
- [Soal 5](#soal-5)


## Soal 1

Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.



Yang harus dikerjakan di soal ini adalah mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png” dengan program c.


```c
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

void delext (char *name)
{
    char *back = name + strlen(name);
    while (back > name && *back != '.') --back;
    if (back > name) *back = '\0';
}

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    DIR *sdir;
    struct dirent *sdirent;

    char fname[256][256];
    const char *xname;
    char pname[256][256];
    char path[255];
    char spath[255];
    char dpath[255];
    char fpath[255];
    char lname[255];
    int ren, i=0, k=0;

    strcpy(lname, "_grey.png");
    strcpy(path, "/home/sea/Documents/Sisop/Modul_2/Media");
    sdir = opendir(path);
    if (sdir != NULL)
    {
        while ((sdirent=readdir(sdir))!=NULL)
        {
	    strcpy(fname[i],sdirent->d_name);
	    xname = strrchr(sdirent->d_name,'.');
	    if (strcmp(xname,".png")==0)
	    {
		strcpy(spath, "Media/");
    	strcpy(dpath, "gambar/");
		strcpy(pname[k],fname[i]);
		delext(pname[k]);
		strcat(pname[k],lname);
		strcat(spath,fname[i]);
	    strcat(dpath,pname[k]);

		ren = rename(spath,dpath);
		printf ("%d. %s --> %s\n",k , sdirent->d_name, pname[k]);
		if (ren != 0) perror("Error");

		memset(spath, 0, sizeof spath);
		memset(dpath, 0, sizeof dpath);
		k++;
		printf ("%d\n",k);
	    }
	    i++;
	    printf("%d\n",i);
        }
        closedir(sdir);
    }
  }
}



```


Program diatas menggunakan daemon agar berjalan secara otomatis. Dimulai dengan membuka folder yang memiliki file berekstensi .png dengan menggunakan library <dirent.h>. Lalu tiap nama file yang berada di folder tersebut disimpan di sebuat array dua dimensi yang bernama fname. Lalu dengan menggunakan strrchr() dan strcmp(), di cocokkan ekstensi tiap file yang berada di folder tersebut, jika file tersebut berkestensi .png, maka nama file tersebut disimpan di array lain bernama pname. Lalu pathnya digabung-gabungkan sehingga membentuk sebuah path asal (spath) dan path tujuan (dpath) untuk dimasukkan ke fungsi rename. Isi dari spath adalah path dari file yang akan diubah, sedangkan dpath berisi path dari nama file yang sudah ditambahkan "_grey.png". Setelah di rename, array spath dan dpath dikosongkan agar dapat dipakai untuk file selanjutnya.  

## Soal 2

Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab

Yang harus dikerjakan di soal ini adalah menghapus file elen.ku tiap 3 detik yang berada di folder hatiku dengan mendeteksi owner dan group dari file tersebut jika sama dengan "www-data" maka data tersebut dihapus.


```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>


int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
	sleep(3);
	struct group *gr;
        struct passwd *pw;

	DIR *sdir;
	struct dirent *sdirent;

        char fname[255][255];
	char pname[255][255];
	char elen[10];
	strcpy(elen,"elen.ku");
	int i=0,k=0;
	char path[255];

	sdir = opendir("/home/sea/Documents/Sisop/Modul_2/hatiku");
	if(sdir != NULL){
		while ((sdirent=readdir(sdir)) != NULL){
			strcpy(path,"/home/sea/Documents/Sisop/Modul_2/hatiku/");
			strcpy(fname[i],sdirent->d_name);
			if(strcmp(fname[i],"elen.ku")==0){
				struct stat info;
				strcpy(pname[k],fname[i]);
				strcat(path,pname[k]);
				chmod(path, S_IRWXU|S_IRWXG|S_IRWXO);
				stat(path, &info);

				uid_t uid=info.st_uid;
				gid_t gid=info.st_gid;
				gr = getgrgid(gid);
				pw = getpwuid(uid);
				printf ("%s : %s , %s\n",pname[i],gr->gr_name,pw->pw_name);


				if(strcmp(gr->gr_name,"www-data")==0 && strcmp(pw->pw_name,"www-data")==0) remove (path);
				memset(path, 0,sizeof path);
				k++;
			}
			i++;
		}
	}
	closedir(sdir);
  }
}

```
Program c diatas menggunakan daemon agar file elen.ku terhapus setiap 3 detik. Dimulai dari membuka folder dengan menggunakan library <dirent.h>, lalu di simpan tiap nama file yang ada di folder tersebut. Jika nama file tersebut adalah "elen.ku" maka file tersebut disimpan di array pname. Lalu file tersebut diubah permissionnya menjadi 0777 dengan fungsi chmod. Lalu diambil id user dan id group dari file tersebut menggunakan stat. Jika sudah ketemu id nya maka bisa dicari nama usernya dengan menggunakan getpwuid dan getgrgid untuk mengambil nama groupnya. Lalu nama group dan nama usernya di compare dengan mengguanakan strcmp apakah sama dengan "www-data" atau tidak. Jika sama maka file tersebut dihapus dengan fungsi remove.

## Soal 3
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
i)  mengekstrak file zip tersebut.
ii) menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
Catatan:  
- Gunakan fork dan exec.
- Gunakan minimal 3 proses yang diakhiri dengan exec.
- Gunakan pipe
- Pastikan file daftar.txt dapat diakses dari text editor

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <dirent.h>
#include <sys/wait.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main() {
  pid_t child_id;
  int fd[2];
  int i=0,k=0;
  int status;

  DIR *sdir;
  struct dirent *sdirent;

  FILE *f;

  char fname[255][255];
  char pname[255];
  const char *xname;
  
	
  if(pipe(fd)==-1) die("pipe");

  child_id = fork();

  if (child_id == 0) {
  	pid_t grchild_id;
  	

  	grchild_id = fork();

  	if (grchild_id == 0){
		
		pid_t grgrchild_id;
		grgrchild_id = fork();

		if(grgrchild_id == 0){
			//this is grand grand child
			char *argv[3]={"unzip","campur2.zip",NULL};
			execv("/usr/bin/unzip",argv);
		}else{
			//this is grandchild
			wait(NULL);
	
			char *argv[5]={"touch","daftar.txt",NULL};
			execv("/usr/bin/touch",argv);
			die("execv");
						
		}
    	}else{
		//this is child
		wait(NULL);

		close(fd[0]);
		
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		char *argv[5]={"find","campur2/","-name","*.txt",NULL};
		execv("/usr/bin/find",argv);
		
	}


  } else {
  	// this is parent
	wait(NULL);

	close(fd[1]);
	read(fd[0], pname, sizeof(pname));
	close(fd[0]);
	f = fopen("daftar.txt","w");
	fputs(pname,f);
	fclose(f);
	
  }
   return 0;
}
```
Perintah di soal ini sudah jelas dituliskan sesuai seperti di soal. Soal meminta untuk diadakannya menggunakan fork dan exec. Program diatas memiliki grand grand child, yaitu parent memiliki child1, child1 memiliki child2 dan child2 memiliki child3. Hal tersebut dikarenakan dibutuhkannya 3 proses yang berakhiran exec seperti yang diminta di soal. Di program ini pipenya adalah fd. Exec pertama adalah unzip file campur2.zip di grand grand child. Grand child mengunggu grand grand child untuk menyelesaikan tugasnya, jika sudah selesai grand child membuat file .txt kosong yang bernama "daftar.txt". Child menunggu grand child untuk menyelesaikan tugassnya lalu menutup pipe fd[0](read), dan memasukkan output yang akan dihasilkan oleh exec dibawahnya ke fd[1](write) dengan menggunakan dup2, lalu menutup pipe fd[1] dan exec find dijalankan untuk mengambil nama file yang berekstensi ".txt". Parent menunggu proses yang di child selesai lalu parent menutup pipe fd[1] hasil dari child, lalu membaca output tersebut dengan memasukkan output yang disimpan di pipe ke array pname, lalu menutup pipe fd[0]. Selanjutnya, dengan menggunakan fopen, output yang ada di pname dimasukkan ke file pointer f, lalu file ditutup.

## Soal 4

Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh:
File makan_enak.txt terakhir dibuka pada detik ke-1
Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: 
- dilarang menggunakan crontab
- Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

Soal ini memerintahkan untuk membuat program c yang bisa membuat file makan_sehat#.txt dengan # adalah integer yang bertambah terus sesuai dengan pertambahan file. Jika file makan_enak telah diakses dalam waktu kurang dari 30 detik yang lalu. Program ini berjalan tiap 5 detik.

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main(){
	  pid_t pid, sid;

  	  pid = fork();

  	  if (pid < 0) {
  	  exit(EXIT_FAILURE);
	  }

	  if (pid > 0) {
	    exit(EXIT_SUCCESS);
	  }

	  umask(0);

	  sid = setsid();
	
	  if (sid < 0) {
	    exit(EXIT_FAILURE);
	  }

	  if ((chdir("/home/sea/Documents/Sisop/Modul_2/makanan")) < 0) {
	    exit(EXIT_FAILURE);
	  }

	  close(STDIN_FILENO);
	  close(STDOUT_FILENO);
	  close(STDERR_FILENO);	
	  
	  while (1){

		FILE *f;
        	struct stat filestat;
		char fp[255]="makan_enak.txt";
		char baru[255];
		long int act,ctm,sub;
		
        	stat(fp,&filestat);
		time_t act = filestat.st_atime;
		ctm = time(NULL);
		sub = ctm - act;	
		
		int i=1;
		if(sub<30){
				
			while(1){
				sprintf(baru,"Makan_sehat%d.txt",i);
				if(fopen(baru,"r")==0){
					f=fopen(baru,"w+");
					break;
				}	
				i++;
			}
		
		}
		sleep (5);

	   }
	
	
}

``` 

Program ini menggunakan daemon dikarenakan soal meminta untuk menjalankan program ini setiap 5 detik. Lalu dengan stat dapat diambil waktu terakhir diakses dengan st_atime. Output dari integer act dan ctm adalah jumlah waktu dalam detik. Lalu waktu keduanya dikurangin dan hasilnya disimpan di integer sub. Jika sub kurang dari 30 maka akan dibuat file makan_sehat dengan increment # sebelum ekstensi. Jika file sudah ada maka akan di loop di dalam for sampai ketika dicek file tidak ada. Jika file tidak ada maka makan_sehat#.txt dibuat dan loop berhenti.  

  
## Soal 5

Kerjakan poin a dan b di bawah:
- Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log
Ket:
Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
Per menit memasukkan log#.log ke dalam folder tersebut
‘#’ : increment per menit. Mulai dari 1

- Buatlah program c untuk menghentikan program di atas.

NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <locale.h>

void delext (char *name)
{
    char *back = name + strlen(name);
    while (back > name && *back != '.') --back;
    if (back > name) *back = '\0';
}

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    FILE *fr, *fw;
	time_t times;
	struct tm *timeptr;
	char buf[100];
	char waktu[255];

	char path[255];
	char ext[10];
	char pathh[255];
	char ang[50];
	char C;

	int t=1800;

	strcpy(path,"/home/sea/Documents/Sisop/Modul_2/");

	sleep(t);
	setlocale(LC_ALL,"/QSYS.LIB/EN_US.LOCALE");
	times = time(NULL);
	timeptr = localtime(&times);
	strftime(buf,sizeof(buf), "%d:%m:%Y-%H:%M", timeptr);
	mkdir(buf,0777);
	strcat(path,buf);
	strcat(path,"/");
	


	int x=60;
	while(1){
		fr = fopen("/var/log/syslog","r");
		char baru[255]="";
		sprintf(baru,"log%d.log",x);
		strcpy(pathh,path);
		strcat(pathh,baru);
		fw = fopen(pathh,"w+");


		C = fgetc(fr);
		while(C != EOF)
		{
			fputc(C, fw);
			C = fgetc(fr);
		}
		x++;
		fclose(fr);
		fclose(fw);

		sleep(1);
		if(x>30) break;

	}

	memset (pathh, 0 ,sizeof (pathh));
  
}
}

```
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>

int main()
{
        char *argv[3]= {"killall","soal5",NULL};
        execv ("/usr/bin/killall",argv);
}

```
Di soal ini diperintahkan untuk mengcopy isi file dari syslog ke file log#.log (dengan # increment) dan memasukkannya ke folder yang diberi nama sesuai dengan waktu. Folder akan dibuat setiap 30 menit (1800 sec) dan file akan dibuat tiap menit. 
Pertama, file dibuka dan dibaca isinya dengan menggunakan fopen r, lalu dimasukkan ke path yang sudah di concat terlebih dahulu agar sesuai dengan path file baru yang akan dibuat. Lalu file syslog di write dengan menggunakan fopen w+ dan diinputkan per char ke fw. Incrementkan x lalu close fr dan fw. Karena folder akan dibuat tiap 30 menit maka tiap folder akan berisi 30 file, sehingga jika x=30 maka loopnya berhenti. Kosongkan array pathh agar bisa dipakai lagi.

