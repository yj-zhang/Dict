#include <stdio.h>
#include <string.h>
//#define _DEBUG 0
#define MAXLEN_DATA 256


 
/*��ѯ����*/
int main()
{
    char buff[MAXLEN_DATA]={0};
    FILE *fp;
    char *buf = buff;
    int i= 0;
    char word1[35]="\0", word2[35] = "\0";

    /*���ҵ���*/
    if ((fp = fopen("dict.txt", "r")) == NULL)
	{
        perror("fail to open");
        return -1;
    }

  
    while (fgets(buff, MAXLEN_DATA, fp) != NULL)
	{
		//��ȡ����
		 while (*(++buf) != ' ')
		{
			i++;
            continue;//������һ������
        }	
		
		strncpy(word1,buff,i);
		word1[i] = '\0';
		
		//��ȡ����
		while (*(++buf) == ' ')
		{
            continue;//�������ʺ���Ŀո�
        }
//		strcpy(meaning,buf);
		
	//	sprintf(sqlstr,"insert into word values ('%s','%s')",word1,meaning);
	//	sqlite3_exec();
    }

   
    return 0;
}