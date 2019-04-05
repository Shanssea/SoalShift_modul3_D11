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
