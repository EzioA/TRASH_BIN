#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "nec_ds.h"

int init_lib(void *addr)
{
	int i;
	FILE *file = NULL;
	char *name[_BOOK_MAX] = {"Hello Linux",
	"Math",
	"The C Programming",
	"How to Read",
	"The Man who Changed this Country"};
	char *index[_BOOK_MAX] = {"STD001",
	"STD002",
	"STD003",
	"STD004",
	"STD005"};
	struct bookmap Map[_BOOK_MAX];
	struct bookmap* temp = (struct bookmap *)addr;

	if (!(file = fopen("./library", "r+")))	//insure that the file isn't NULL
	{
		int list[_BOOK_MAX] = {0};
		file = fopen("./library", "w+");
		printf("Please input the count of each book\n");
		for (i = 0; i <= 4; i++)
		{
			do
			{
				printf("%s:", name[i]);
				scanf("%d", &list[i]);
			} while (list[i] <= 0);
			Map[i].count = list[i];
			strncpy(Map[i].index, index[i], _INDEX_MAX);
			strncpy(Map[i].name, name[i], _NAME_MAX);
		}
		fwrite(&Map, sizeof(struct bookmap), _BOOK_MAX, file);
		fclose(file);
		file = fopen("./library", "r+");
	}
	fread(&Map, sizeof(struct bookmap), _BOOK_MAX, file);	//get data from file

//	*temp = *Map;
	for (i = 0; i <= 4; i++)
		temp[i] = Map[i];	//copy data from file to shm
//	printf("%s\n", temp[2].name);
//	printf("%x\n", temp);
	fclose(file);
	return 0;
}

int main(int argc, char **argv)
{
	union semun arg;
	int semid;
	int shmid;
	void *addr = NULL;

	semid = semget(KEY, _BOOK_MAX, IPC_CREAT | 0666);  //create sem set
	shmid = shmget(KEY, sizeof(struct bookmap) * _BOOK_MAX, IPC_CREAT | 0666);						 	  //create shm

	unsigned short array[_BOOK_MAX] = {1, 1, 1, 1, 1};
	arg.array = array;	//for initializing the sem set(each with value of 1)
	semctl(semid, 0, SETALL, arg);

	addr = shmat(shmid, 0, 0);	//get shm attached


	init_lib(addr);

//	struct bookmap* temp = (struct bookmap *)addr;
//	printf("%s\n", temp[0].name);
//	printf("%x\n", temp);

	shmdt(addr);

	return 0;
}
