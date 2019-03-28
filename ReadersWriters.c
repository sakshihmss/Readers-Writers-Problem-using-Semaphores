#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, wrt;
int data = 0, readcnt = 0;

void *reader(void *arg){
	int f;
	f = ((intptr_t)arg);
	sem_wait(&mutex);
	readcnt += 1;
	if (readcnt == 1)
		sem_wait(&wrt);
	sem_post(&mutex);
	printf("Data read by the reader%d is %d\n",f,data);
	sleep(1);
	sem_wait(&mutex);
	readcnt -= 1;
	if(readcnt == 0)
		sem_post(&wrt);
	sem_post(&mutex);
}

void *writer(void *arg){
	int f;
	f = ((intptr_t)arg);
	sem_wait(&wrt);
	data++;
	printf("Data written by the writer%d is %d\n",f, data);
	sleep(1);
	sem_post(&wrt);
}

int main(){
	int i, b;
	pthread_t rtid[5], wtid[5];
	sem_init(&mutex, 0, 1);
	sem_init(&wrt, 0, 1);
	for (i=0;i<=2;i++){
		pthread_create(&wtid[i], NULL, writer, (void *)(intptr_t)i);
		pthread_create(&rtid[i], NULL, reader, (void *)(intptr_t)i);
	}
	for (i=0;i<=2;i++){
		pthread_join(wtid[i], NULL);
		pthread_join(rtid[i], NULL);
	}
	return 0;
}
