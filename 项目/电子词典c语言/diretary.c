#include <stdio.h>
#include <string.h>
//#define _DEBUG 0
#define MAXLEN_DATA 256


void menu();

/*��ѯ����*/
int fsearch()
{
    char buff[MAXLEN_DATA]={0};
    FILE *fp;
    char *buf = buff;
    int i= 0;
	int flag = 0;
    char word1[35]="\0", word2[35] = "\0";

    /*���ҵ���*/
    if ((fp = fopen("dict.txt", "r")) == NULL)
	{
        perror("fail to open");
        return -1;
    }
	printf("��������Ҫ���ҵĵ��ʣ�\n>");
	scanf("%s",word1);
    while (word1[i] != '\0')
	{//תΪСд
        if (word1[i] >= 'A' && word1[i] <= 'Z')
		{
            word1[i] += 32;
        }
        i++;
    }
    /*ĩβ���ո�  Ψһȷ��һ������*/
    word1[i] = ' ';
    word1[i+1] = '\0';
#if 1  
    i = 0;
    while (word1[i] != '\0')
	{
        word2[i] = word1[i]-32;//��д
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

    if (flag == 1)//�ж���û���ҵ�����
	{//find word
#ifdef _DEBUG
        printf("%s", buff);
#endif
		//�ҵ�����
  
        while (*(++buf) != ' ')
		{
            continue;//������һ������
        }
        while (*(++buf) == ' ')
		{
            continue;//�������ʺ���Ŀո�
        }
        printf("%s \n",buf);
    } else 
	{//û�ҵ�
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
		menu();			//�˵���ʾ
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
		default:                        //���������������
			printf("invalid command, try again\n");
			break;
		}

	
	}
	
	return 0;
}
//ϵͳ�˵�
void menu()
{
	printf("\n");
	printf("|===================================================\n");
	printf("|1: search word\n");
	 
	printf("|4: exit\n");
	printf("|====================================================\n");
}