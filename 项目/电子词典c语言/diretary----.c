#include <stdio.h>
#include <string.h>
//#define _DEBUG 0
#define MAXLEN_DATA 256


 
/*查询单词*/
int main()
{
    char buff[MAXLEN_DATA]={0};
    FILE *fp;
    char *buf = buff;
    int i= 0;
    char word1[35]="\0", word2[35] = "\0";

    /*查找单词*/
    if ((fp = fopen("dict.txt", "r")) == NULL)
	{
        perror("fail to open");
        return -1;
    }

  
    while (fgets(buff, MAXLEN_DATA, fp) != NULL)
	{
		//提取单词
		 while (*(++buf) != ' ')
		{
			i++;
            continue;//跳过第一个单词
        }	
		
		strncpy(word1,buff,i);
		word1[i] = '\0';
		
		//提取解释
		while (*(++buf) == ' ')
		{
            continue;//跳过单词后面的空格
        }
//		strcpy(meaning,buf);
		
	//	sprintf(sqlstr,"insert into word values ('%s','%s')",word1,meaning);
	//	sqlite3_exec();
    }

   
    return 0;
}