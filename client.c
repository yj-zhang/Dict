#include<dict_sevr.h>


 int login(int);
int logon(int);
int search_sys(int, int);

/*一级菜单*/
int menu_1(int *sel)
{

	while(1)
	{
		system("clear");
		printf("\t\t|+++++++++++++++++++++++|\n");
		printf("\t\t|	1、登录		|\n");
		printf("\t\t|	2、注册		|\n");
		printf("\t\t|	3、退出		|\n");
		printf("\t\t|+++++++++++++++++++++++|\n");

		scanf("%d", sel);
		getchar(); //吃掉回车
		if(*sel > 0 && *sel < 4 )
		{
			return 0;
		}

		printf("输入错误,请按回车重新输入\n");
		getchar();
		continue;

	}
}

int menu_2(int *sel)
{

	while(1)
	{
		system("clear");
		printf("\t\t|+++++++++++++++++++++++|\n");
		printf("\t\t|	1、查词  	|\n");
		printf("\t\t|	2、历史记录	|\n");
		printf("\t\t|	3、个人信息	|\n");
		printf("\t\t|	4、退出  	|\n");
		printf("\t\t|+++++++++++++++++++++++|\n");
		*sel = 0;
		scanf("%d", sel);
	//	fflush(stdin);
	//	printf("执行了一次\n");
		getchar(); //吃掉回车
		if(*sel > 0 && *sel < 5 )
		{
			return 0;
		}

		printf("输入错误,请按回车重新输入\n");
		getchar();
		

	}

}

int main()
{
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(6666);
	//my_addr.sin_addr.s_addr = inet_addr("192.168.1.117");
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t addrlen;
	addrlen = sizeof(my_addr);
	int sevr_fd;
	int sel; //一级菜单选择

	if((sevr_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket failed");
		return -1;
	}
	printf("sevr_fd = %d\n",sevr_fd);
	if(connect(sevr_fd, (struct sockaddr *)&my_addr, addrlen) == -1)
	{
		perror("connect failed");
		return -1;
	}
	//	printf("链接成功\n");
	while(1)
	{
		menu_1(&sel);
		switch(sel)
		{
		case 1:
			{
				send(sevr_fd, (void *)&sel, sizeof(int), 0);
				login(sevr_fd);
				break;
			}
		case 2:
			{
				send(sevr_fd, (void *)&sel, sizeof(int), 0);
				logon(sevr_fd);
				break;
			}
		default:
			send(sevr_fd, (void *)&sel, sizeof(int), 0);
			return 0;
		}
	}

	close(sevr_fd);
}
int login(int sevr_fd)
{
	usr_info usr_login;
	int send_byte;
	int try_num;
	int sel;
	sevrpack rcvpack;

	for(try_num = 0; try_num < 3; try_num++)//尝试三次登录
	{
		bzero(&usr_login, sizeof(usr_login));
		printf("请输入用户名：\n");
		scanf("%s", usr_login.usr_name);
		printf("请输入密码：\n");
		scanf("%s", usr_login.password);
		//send_byte = send(sevr_fd, (void *)(&usr_login), strlen((char *)(&usr_login)), 0);
		send_byte = send(sevr_fd, (void *)(&usr_login), sizeof(usr_info), 0);
		if(send_byte == -1)
		{
			perror("send failed");
			return -1;
		}
		if(send_byte == 0)
		{
			perror("server disconnection");
			return -1;
		}
		bzero(&rcvpack, sizeof(sevrpack));
		recv(sevr_fd, (void *)&rcvpack, sizeof(sevrpack), 0);
		//	printf("%d\n",rcvpack.type);
		switch(rcvpack.type)
		{
		case ABSENT:
			printf("登录失败：%s\n",rcvpack.msg);
			//sleep(2);
			break;
		case CHECK_FAIL:
			printf("登录失败：%s\n",rcvpack.msg);
			break;
		case CHECK_SUCCESS://显示二级菜单
		  	//printf("显示二级菜单\n");
			while(1)
			{
				menu_2(&sel);
				if(sel == 4)
				{
					send(sevr_fd, (void *)&sel, sizeof(int), 0);
					return 0;
				}
				else
				{
					search_sys(sel,sevr_fd);
				printf("输入回车继续\n");
				getchar();
				}
			}


		default:
			break;

		}
	}
	printf("您已经输错3次\n");
}
int logon(int sevr_fd)
{
	usr_info usr_logon;
	int send_byte;
	sevrpack rcvpack;
	bzero(&usr_logon, sizeof(usr_logon));
	printf("请输入用户名：\n");
	scanf("%s", usr_logon.usr_name);
	printf("请输入密码：\n");
	scanf("%s", usr_logon.password);
	send_byte = send(sevr_fd, (void *)(&usr_logon), sizeof(usr_info), 0);
	if(send_byte == -1)
	{
		perror("send failed");
		return -1;
	}
	if(send_byte == 0)
	{
		perror("server disconnection");
		return -1;
	}
	bzero(&rcvpack, sizeof(sevrpack));
	recv(sevr_fd, (void *)&rcvpack, sizeof(sevrpack), 0);
	switch(rcvpack.type)
	{
	case ABSENT:
		printf("注册失败：%s\n",rcvpack.msg);
		//sleep(2);
		break;
	case CHECK_SUCCESS://注册成功
		printf("注册成功\n");
		break;

	default:
		break;

	}


}
int search_sys(int sel,int sevr_fd)
{
	int send_byte;
	char word[32];
	int i = 0;
	int nrow = 0;//接收服务器传回来的历史记录的条数
	translate client_rcv;
	switch(sel)
	{
	case 1:
		send(sevr_fd, (void *)&sel, sizeof(int), 0);
		//输入查询的单词并发送给服务器
	
		printf("请输入要查询的单词：");
		fflush(stdout);//这里刷新缓冲区来把上面printf里的内容输出，因为上面printf没加\n，这样的目的是使输入光标停在提示语后面
		bzero(word, sizeof(word));
		scanf("%s", word);
		getchar();
		int byte;
		byte = send(sevr_fd, (void *)word, strlen(word)+1, 0);
		printf("要查询的词是%s\n",word);
		recv(sevr_fd, (void *)&client_rcv, sizeof(client_rcv), 0);
		if(client_rcv.type == CHECK_SUCCESS)
		{
			printf("%s  \t%s\n",client_rcv.word,client_rcv.mean);

			/* 转换成大写*/
			while(client_rcv.word[i] != '\0')
			{
				client_rcv.word[i] = client_rcv.word[i] - 32;
				i++;
			}
			printf("%s  \t%s\n",client_rcv.word,client_rcv.mean);
		}
		else
		{
			printf("未查询到相关单词\n");
		}
		break;
		


		//接收服务器传回来的结果
	case 2:
		printf("已经进入历史查询选项\n");
		send(sevr_fd, (void *)&sel, sizeof(int), 0);
		printf("发送选项完毕\n");
		recv(sevr_fd, (void *)&nrow, sizeof(int), 0);
		printf("nrow=%d\n",nrow);

		for(i = 0; i < nrow; i++)
		{
			recv(sevr_fd, (void *)word, sizeof(word), 0);
			printf("%s\n", word);
		}
		printf("已经跳出for循环\n");
		//查看查询的历史记录
		break;
	case 3:
		send(sevr_fd, (void *)&sel, sizeof(int), 0);
		//修改个人信息
		break;
	default:
		break;
	}
}
