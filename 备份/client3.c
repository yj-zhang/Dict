#include<dict_sevr.h>
#include<signal.h>
int login(int);
int logon(int);
int search_sys(int, int);

void my_func(int sign_no)
{
	if(sign_no == SIGPIPE)
	{
		printf("i have got SIGPIPE\n");
	}
}

/*一级菜单*/
int menu_1(int *sel)
{
	
	while(1)
	{
		system("clear");
		*sel = 0;	
		printf(SET_COLOR"\t\t|+++++++++++++++++++++++|\n"END_SET);
		printf(SET_COLOR"\t\t|	1、登  录	|\n"END_SET);
		printf(SET_COLOR"\t\t|	2、注  册	|\n"END_SET);
		printf(SET_COLOR"\t\t|	3、退  出	|\n"END_SET);
		printf(SET_COLOR"\t\t|+++++++++++++++++++++++|\n"END_SET);

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
		*sel = 0;
		printf(SET_COLOR"\t\t|+++++++++++++++++++++++|\n"END_SET);
		printf(SET_COLOR"\t\t|	1、查  词       |\n"END_SET);
		printf(SET_COLOR"\t\t|	2、历 史 记 录  |\n"END_SET);
		printf(SET_COLOR"\t\t|	3、个 人 信 息  |\n"END_SET);
		printf(SET_COLOR"\t\t|	4、退  出       |\n"END_SET);
		printf(SET_COLOR"\t\t|+++++++++++++++++++++++|\n"END_SET);
		
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

int menu_3(int *sel)
{
	while(1)
	{
		system("clear");
		*sel = 0;
		printf(SET_COLOR"\t\t|+++++++++++++++++++++++|\n"END_SET);
		printf(SET_COLOR"\t\t|	1、查看个人信息 |\n"END_SET);
		printf(SET_COLOR"\t\t|	2、修改个人信息 |\n"END_SET);
		printf(SET_COLOR"\t\t|	3、退  出	|\n"END_SET);
		printf(SET_COLOR"\t\t|+++++++++++++++++++++++|\n"END_SET);

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

int main()
{
	struct sigaction action;
	sigaction(SIGPIPE, 0, &action);
	action.sa_handler = my_func;
	sigaction(SIGPIPE, &action, 0);



	struct sockaddr_in sevr_addr;
	bzero(&sevr_addr, sizeof(sevr_addr));
	sevr_addr.sin_family = AF_INET;
	sevr_addr.sin_port = htons(6666);
	//my_addr.sin_addr.s_addr = inet_addr("192.168.1.117");
	sevr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	
	int sevr_fd;
	int sel; //一级菜单选择
	int exit_status = 0;
	if((sevr_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket failed");
		return -1;
	}
	printf("sevr_fd = %d\n",sevr_fd);
	if(connect(sevr_fd, (struct sockaddr *)&sevr_addr, sizeof(sevr_addr)) == -1)
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
				printf("已经发送\n");
				/*		if(state < 0)
						{
						printf("与客户端连接出错\n");
						return -1;	
						}
						else if(0 == state)
						{
						printf("客户端关闭链接\n");
						return -1;
						}
						*/
				exit_status = login(sevr_fd);
		//		printf("exit_status= %d\n", exit_status);
				break;
			}
		case 2:
			{
				send(sevr_fd, (void *)&sel, sizeof(int), 0);
				/*	if(state < 0)
					{
					printf("与客户端连接出错\n");
					return -1;	
					}
					else if(0 == state)
					{
					printf("客户端关闭链接\n");
					return -1;
					}
					*/
				exit_status = logon(sevr_fd);
				break;
			}
		default:
			send(sevr_fd, (void *)&sel, sizeof(int), 0);
			return 0;
		}
		if(exit_status == -1)
			break;
	}
//	printf("即将退出\n");
	close(sevr_fd);
}
int login(int sevr_fd)
{
	usr_info usr_login;
	int state;
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
//		printf("password = %s\n",usr_login.password);
		//send_byte = send(sevr_fd, (void *)(&usr_login), strlen((char *)(&usr_login)), 0);
		send(sevr_fd, (void *)(&usr_login), sizeof(usr_info), 0);

		printf("已经发送完用户名和密码\n");
		/*	if(send_byte == -1)
			{
			perror("send failed");
			return -1;
			}
			if(send_byte == 0)
			{
			perror("server disconnection");
			return -1;
			}
			*/	

		bzero(&rcvpack, sizeof(sevrpack));
		state = recv(sevr_fd, (void *)&rcvpack, sizeof(sevrpack), 0);
		if(state == -1)
		{
			printf("send failed\n");
		//	sleep(5);
			return -1;
		}
		if(state == 0)
		{

			printf("server disconnection\n");
		//	sleep(5);
			return -1;
		}

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
				//	printf("输入回车继续\n");
				//	getchar();
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
	int state;
	sevrpack rcvpack;
	bzero(&usr_logon, sizeof(usr_logon));
	printf("请输入用户名：\n");
	scanf("%s", usr_logon.usr_name);
	printf("请输入密码：\n");
	scanf("%s", usr_logon.password);
	send(sevr_fd, (void *)(&usr_logon), sizeof(usr_info), 0);
	/*	if(send_byte == -1)
		{
		perror("send failed");
		return -1;
		}
		if(send_byte == 0)
		{
		perror("server disconnection");
		return -1;
		}
		*/
	bzero(&rcvpack, sizeof(sevrpack));
	state = recv(sevr_fd, (void *)&rcvpack, sizeof(sevrpack), 0);
	if(state == -1)
	{
		printf("send failed\n");
		return -1;
	}
	if(state == 0)
	{
		printf("server disconnection\n");
		return -1;
	}
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
	int state;
	char word[32];
	int i = 0;

	int nrow = 0;//接收服务器传回来的历史记录的条数
	translate client_rcv;
	usr_info user_data;//个人信息

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

		send(sevr_fd, (void *)word, strlen(word)+1, 0);
		printf("要查询的词是%s\n",word);
		state = recv(sevr_fd, (void *)&client_rcv, sizeof(client_rcv), 0);
		if(state == -1)
		{
			printf("send failed\n");
			return -1;
		}
		if(state == 0)
		{
			printf("server disconnection\n");
			return -1;
		}
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
		printf("输入回车继续\n");
		getchar();
		break;



		//接收服务器传回来的结果
	case 2:


		printf("已经进入历史查询选项\n");
		send(sevr_fd, (void *)&sel, sizeof(int), 0);
		printf("发送选项完毕\n");
		state = recv(sevr_fd, (void *)&nrow, sizeof(int), 0);
		if(state == -1)
		{
			printf("send failed\n");
			return -1;
		}
		if(state == 0)
		{
			printf("server disconnection\n");
			return -1;
		}
		printf("nrow=%d\n",nrow);

		for(i = 0; i < nrow; i++)
		{
			state = recv(sevr_fd, (void *)word, sizeof(word), 0);
			if(state == -1)
			{
				printf("send failed\n");
				return -1;
			}
			if(state == 0)
			{
				printf("server disconnection\n");
				return -1;
			}
			printf("%s\n", word);
		}
		printf("已经跳出for循环\n");
		//查看查询的历史记录
		printf("输入回车继续\n");
		getchar();
		break;
	case 3:
		send(sevr_fd, (void *)&sel, sizeof(int), 0);
		printf("已经进入第三选项\n");
		while(1)
		{
			menu_3(&sel);
			printf("已经由第三菜单返回\n");
			switch(sel)
			{
			case 1:
				send(sevr_fd, (void *)&sel, sizeof(int), 0);
				state = recv(sevr_fd, (void *)&user_data, sizeof(usr_info), 0);
				if(state == -1)
				{
					printf("send failed\n");
					return -1;
				}
				if(state == 0)
				{
					printf("server disconnection\n");
					return -1;
				}
				printf("用户名: %s\t密码: %s\n", user_data.usr_name, user_data.password);
				printf("输入回车继续\n");
				getchar();
				break;
			case 2:
				send(sevr_fd, (void *)&sel, sizeof(int), 0);
				printf("请输入新的用户名\n");
				scanf("%s",user_data.usr_name);
				printf("请输入密码\n");
				scanf("%s",user_data.password);
				getchar();
				send(sevr_fd, (void *)&user_data, sizeof(usr_info), 0);

				break;
			case 3:
				send(sevr_fd, (void *)&sel, sizeof(int), 0);
				
				break;

			}	
			//修改个人信息
			if(sel == 3)
				break;
		}
	default:
		break;

	}
}
