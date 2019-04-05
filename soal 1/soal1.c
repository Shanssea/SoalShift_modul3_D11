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

int main (void){
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
