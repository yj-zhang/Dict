#define SIZE 3
#define _DEBUG 1
#define MAXLEN_DATA 40

struct vote
{    
     char name[20];
     int count;
};

struct user
{ 
     char name[20];
	 char password[20];
	 int rest;
};
#define DEBUG_VOTE_ 0