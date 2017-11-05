/*实现将文本的词典导入数据库*/
#include<stdio.h>
#include<sqlite3.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
#define MAX_LEN 1024

int main()
{
	sqlite3 *db;
	char *errmsg;
	FILE *dict_fd;
	char buff[MAX_LEN] = {0};
	char *buf = buff;
	char word1[512] = {'\0'};
	char meaning[1024] = {'\0'};
	char sqlstr[1024] = {0};
	int i = 0;

#if(0)
	if(sqlite3_open("dict.db", &db) != SQLITE_OK)
	{
		printf("error: %s\n",sqlite3_errmsg(db));
		exit(-1);
	}
	if(sqlite3_exec(db, "create table if not exists usr(name text not null,password text not null)", NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("error: %s\n",errmsg);
		exit(-1);

	}
#endif
//	#if(1)
	if(sqlite3_open("dict.db", &db) != SQLITE_OK)
	{
		printf("error: %s\n",sqlite3_errmsg(db));
		exit(-1);
	}
	if(sqlite3_exec(db, "create table if not exists word(word text not null,meaning text not null)", NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("error: %s\n",errmsg);
		exit(-1);

	}
	
	if((dict_fd = fopen("dict.txt", "r")) == NULL)
	{
		perror("error:");
		exit(-1);
	}
	while(fgets(buff, MAX_LEN, dict_fd) != NULL)
	{
		i = 0;
		buf = buff;
		while(*buf != ' ')
		{
			buf++;
			i++;
			continue;
		}
		strncpy(word1, buff, i);
		word1[i] = '\0';
		while(*buf == ' ')
		{
			buf++;
			continue;
		}
		strcpy(meaning, buf);
		sprintf(sqlstr, "insert into word values('%s','%s')", word1,meaning);
		if(sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("error: %s\n", errmsg);
			exit(-1);
		}
	}
	fclose(dict_fd);
//	#endif
	sqlite3_close(db);
	return 0;
}

