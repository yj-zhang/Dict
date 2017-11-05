#include <stdio.h>
#include <string.h>
//#define _DEBUG 0
#define MAXLEN_DATA 256


void menu();

/*查询单词*/
int fsearch()
{
    char buff[MAXLEN_DATA]={0};
    FILE *fp;
    char *buf = buff;
    int i= 0;
	int flag = 0;
    char word1[35]="\0", word2[35] = "\0";

    /*查找单词*/
    if ((fp = fopen("dict.txt", "r")) == NULL)
	{
        perror("fail to open");
        return -1;
    }
	printf("请输入您要查找的单词！\n>");
	scanf("%s",word1);
    while (word1[i] != '\0')
	{//转为小写
        if (word1[i] >= 'A' && word1[i] <= 'Z')
		{
            word1[i] += 32;
        }
        i++;
    }
    /*末尾补空格  唯一确定一个单词*/
    word1[i] = ' ';
    word1[i+1] = '\0';
#if 1  
    i = 0;
    while (word1[i] != '\0')
	{
        word2[i] = word1[i]-32;//大写
        i++;
    }
    word2[i] = '\0';
#endif
#ifdef _DEBUG
    printf("word1 = %s, word2 = %s\n", word1, word2);
#endif


    while (fgets(buff, MAXLEN_DATA, fp) != NULL)
	{
        if (!strncmp(buff, word1, i-1) ||  !strncmp(buff, word2, i-1))
		{
			flag = 1;
            break;			
		}

    }

    if (flag == 1)//判断有没有找到单词
	{//find word
#ifdef _DEBUG
        printf("%s", buff);
#endif
		//找到单词
  
        while (*(++buf) != ' ')
		{
            continue;//跳过第一个单词
        }
        while (*(++buf) == ' ')
		{
            continue;//跳过单词后面的空格
        }
        printf("%s \n",buf);
    } else 
	{//没找到
		printf("not find word %s \n",word1);
    }
 
    return 0;
}
main()
{
	int select = 0;
 
	
	while (1)
	{
		int status = 0;
		menu();			//菜单显示
		printf("Please select the function: \n#");
		scanf("%d", &select);

		//process_pid = fork();
		
		switch (select)
		{
		case 1:               
			fsearch();
			break;
		 
		case 4:
			exit(0);
			break;
		default:                        //错误命令，给出警告
			printf("invalid command, try again\n");
			break;
		}

	
	}
	
	return 0;
}
//系统菜单
void menu()
{
	printf("\n");
	printf("|===================================================\n");
	printf("|1: search word\n");
	 
	printf("|4: exit\n");
	printf("|====================================================\n");
}