#ifndef NEC_DS
#define NEC_DS

#define _BOOK_MAX 5
#define _ALL _BOOK_MAX + 1
#define _NAME_MAX 64 
#define _INDEX_MAX 16
#define KEY 65536

#define _BACK_TO_MAIN 4
#define _AGAIN 5

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

struct bookmap		//the relation within counts, index and name
{
	int count;	//the rest books
	char index[_INDEX_MAX];	
	char name[_NAME_MAX];
};


#endif
