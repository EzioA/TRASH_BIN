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

int P(int, struct sembuf *);
int V(int, struct sembuf *);
int Check();	//check the input
int Checkin();
int Checkout();
int ListLib(int);	//list books under control of arguments
int ListSelf();		//list user library
int List();		//list remote library and user library

void *addr = NULL;	//for usage of writing and reading
int semid;
int shmid;
struct sembuf sop;	//for usage if semop()

int main(int argc, char **argv)
{
	int flag_sem, flag_shm;	//get the return value of semget and shmid
	flag_sem = semid = semget(KEY, _BOOK_MAX, 0 | 0666);  //get the existing sem set
	flag_shm = shmid = shmget(KEY, sizeof(struct bookmap) * _BOOK_MAX, 0 | 0666);  //the same
	if (flag_sem == -1 || !flag_shm == -1)
	{
		printf("Library is not open yet!\n");
		exit(1);
	}
	addr = shmat(shmid, 0, 0);	//attach the shm to current process

	struct bookmap *temp = (struct bookmap *)addr;
//	int *temp = (int *)addr;
//	printf("temp : %x\n", temp);
//	printf("addr : %x\n", addr);
//	printf("%s\n", temp[1].name);

	do
	{
	printf("====>What do you want?<====\n1.Check out\n2.Check in\n3.List\n4.Quit\n>");
	}
	while (Check() == 0);

	return 0;
}

int P(int shmid, struct sembuf *sops)
{
	return semop(shmid, sops, 1);
}

int V(int shmid, struct sembuf *sops)
{
	return semop(shmid, sops, 1);
}

int Check()
{
	int i, r_v;
jmp0:	i = 0;
	scanf("%d", &i);
	getc(stdin);	//get rid of '\n'
//	printf("\n");
	if (i == 1)
	{
		do 
		{
			r_v = Checkout();
			//printf("r_v = %d\n", r_v);
		} while (r_v == _AGAIN);
	}
	else if (i == 2)
	{
		do 
		{
			r_v = Checkin();
			//printf("r_v = %d\n", r_v);
		} while (r_v == _AGAIN);
	}

	else if (i == 3)
		return List();
	else if (i == 4)	//quit
		exit(0);
	else			//fault
	{
		printf("===>please check your input:");
		goto jmp0;
	}
	return 0;
}

int Checkout()
{
	int i, count;
	bool flag_f = false;
	int list[_BOOK_MAX] = {0};
	char str[_NAME_MAX];
	struct bookmap *temp = (struct bookmap *)addr;
	FILE *file = NULL;

	if (!(file = fopen("./records", "r+")))
	{
		//printf("%x\n", file);
		file = fopen("./records", "w+");
		//printf("%x\n", file);
		fwrite(&list, sizeof(int), _BOOK_MAX, file);
		fclose(file);
		file = fopen("./records", "r+");
	}
//	printf("\n");
	
	printf("===>Please input the index or name:");
//	printf("OK!\n");
	fgets(str, _NAME_MAX, stdin);
	str[strlen(str) - 1] = '\0';	//set the terminating character
//	printf("Your input:%s\n", str);

	if (!strcmp(str, "exit"))
	{
		fclose(file);
		return _BACK_TO_MAIN;
	}
	for (i = 0; i <= 4; i++)
	{
		if (!strcmp(str, temp[i].index) || !strcmp(str, temp[i].name))
		{
			flag_f = true;	//strig match
			if (temp[i].count == 0)
			{
				printf("===!There is no %s in the library!===\n\n", str);
				fclose(file);
				return _AGAIN;
			}
			else
			{
				int rval;	//get the return val of scanf()
				sop.sem_num = i;
				sop.sem_op = -1;//oprate the i-th sem of the sem set associated with semid
				sop.sem_flg = SEM_UNDO;
				P(semid, &sop);	//P operation

				ListLib(i);
				do
				{
					printf("===>Input the count:");
					rval = scanf("%d", &count);//if user doesn't unput a number, rval will be 0
					getc(stdin);
				} while (rval == 0 || temp[i].count - count < 0 || count < 0);

				fread(&list, sizeof(int), _BOOK_MAX, file);
				temp[i].count -= count;	//change remote file
				list[i] += count;	
				fseek(file, i * sizeof(int), SEEK_SET);
				fwrite(&list[i], sizeof(int), 1, file); //change lcoal file

				sop.sem_num = i;
				sop.sem_op = 1;
				sop.sem_flg = SEM_UNDO;
				V(semid, &sop);	//V operation

				printf("===!Success!===\n\n");
				break;
			}
		}	
		if (flag_f == false && i == 4)	//no string matches
		{
			fclose(file);
			return _AGAIN;	//call this func again
		}
	}
	fclose(file);
	return 0;
}

int Checkin()
{
	FILE *file = NULL;
	char str[_NAME_MAX];
	struct bookmap *temp = (struct bookmap *)addr;
	int i, count;
	bool flag_f = false;
	int list[_BOOK_MAX] = {0};
	
	if (!(file = fopen("./records", "r+")))
	{
		file = fopen("./records", "w+");
		printf("OK\n");
		fwrite(&list, sizeof(int), _BOOK_MAX, file);
		fclose(file);
		file = fopen("./records", "w+");
	}
	ListSelf();
//	printf("\n");

	fread(&list, sizeof(int), _BOOK_MAX, file);
/*	for (i = 0; i <= 4; i++)
	{
		printf("list[i]:%d\n", list[i]);
	}
*/
	printf("===>Please input the book you want to check in:");
	fgets(str, _NAME_MAX, stdin);
	str[strlen(str) - 1] = '\0';
//	printf("%s\n", str);
	
	if (!strcmp(str, "exit"))
	{
		fclose(file);
		return _BACK_TO_MAIN;
	}
	for (i = 0; i <= 4; i++)
	{
		if (!strcmp(str, temp[i].index) || !strcmp(str, temp[i].name))
		{
			flag_f = true;
			if (list[i] == 0)
			{
				printf("===!You have no %s!===\n\n", str);
				fclose(file);
				return _AGAIN;
			}
			else
			{
				int rval;
				sop.sem_num = i;
				sop.sem_op = -1;
				sop.sem_flg = SEM_UNDO;
				P(semid, &sop);	//P operation

				do
				{
					printf("===>Input the count:");
					rval = scanf("%d", &count);
					getc(stdin);
				} while (rval == 0 || list[i] - count < 0 || count < 0);

				temp[i].count += count;	//change remote file	
				list[i] -= count;	
				fseek(file, i * sizeof(int), SEEK_SET);
				fwrite(&list[i], sizeof(int), 1, file); //local file

				sop.sem_num = i;
				sop.sem_op = +1;
				sop.sem_flg = SEM_UNDO;
				V(semid, &sop);	//V operation

				printf("===!Success!===\n\n");
				break;
			}
		}
		if (flag_f == false && i == 4)
		{
			fclose(file);
			return _AGAIN;
		}
	}
	fclose(file);
	return 0;
}

int ListLib(int index)
{
	int i;
	struct bookmap *temp = (struct bookmap *)addr;

//	printf("temp : %x\n", temp);
//	printf("addr : %x\n", addr);
//	printf("%s\n", temp[0].name);
	if (index == _ALL)
		{
		printf("====>The remote book library:<====\n");
		for (i = 0; i <= 4; i++)
		{
			printf("%-40s%-10s:%d\n", temp[i].name, temp[i].index, temp[i].count);
		}
	}
	else
	{
		printf("====>The book you want:<====\n");
		printf("%-40s%-10s:%d\n", temp[index].name, temp[index].index, temp[index].count);
	}
//	printf("OK!\n");
	return 0;
}

int ListSelf()
{
	int i;
	int list[_BOOK_MAX] = {0};
	struct bookmap *temp = (struct bookmap *)addr;
	FILE *file = NULL;
       
	if (!(file = fopen("./records", "r+")))
	{
		file = fopen("./records", "w+");
		printf("OK\n");
		fwrite(&list, sizeof(int), _BOOK_MAX, file);
		fclose(file);
		file = fopen("./records", "w+");
	}

	fread(&list, sizeof(int), _BOOK_MAX, file);
	printf("====>Your book library:<====\n");
	for (i = 0; i <= 4; i++)
	{
		printf("%-40s%-10s:%d\n", temp[i].name, temp[i].index, list[i]);
	}
	printf("\n");
	fwrite(&list, sizeof(int), _BOOK_MAX, file);
	
	fclose(file);
	return 0;
}

int List()
{
	ListSelf();
	ListLib(_ALL);
	printf("\n");

	return 0;
}
