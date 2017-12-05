#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "nec_ds.h"

int main()
{
	int shmid;
	int semid;
	int flag_sem, flag_shm;
	union semun arg;
	int val, i, flag = 0;
	struct shmid_ds buf;
	FILE *file = NULL;
	void *addr = NULL;

	flag_sem = semid = semget((key_t)KEY, _BOOK_MAX, 0 | 0666);
	flag_shm = shmid = shmget((key_t)KEY, sizeof(struct bookmap) * _BOOK_MAX, 0 | 0666);

	if (flag_sem == -1 || flag_shm == -1)
	{
		printf("The IPC objects you asked doesn't exist!\n");
		exit(1);
	}
	do 
	{
		shmctl(shmid, IPC_STAT, &buf);	
	}	
	while (buf.shm_nattch != 0);		//someone is using the shm

	semctl(semid, 0, IPC_RMID, arg);	//delete this sem set
	file = fopen("./library", "w+");
	addr = shmat(shmid, 0, 0);
	fwrite(addr, sizeof(struct bookmap), _BOOK_MAX, file);  //change remote library
	shmdt(addr);
	shmctl(shmid, IPC_RMID, &buf);		

	fclose(file);
	return 0;
}
