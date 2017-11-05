#include<dict_sevr.h>

int login(int, sqlite3*);
int logon(int, sqlite3 *);
int search_sys(int,sqlite3 *);

/*线程函数*/
void *thrd_func(void * arg)
{
	int clnt_fd;
	clnt_fd = (int)arg;
	sqlite3 *db;
	int sel;
	int state;//用来接收recv和send的返回值
	//打开数据库
	if(sqlite3_open("dict.db", &db) != SQLITE_OK)
	{
		printf("error:%s\n",sqlite3_errmsg(db));
		exit(-1);
	}
	while(1)
	{
		state = recv(clnt_fd, (void *)&sel, sizeof(int), 0);
		if(state < 0)
		{
			printf("与客户端连接出错\n");
			break;	
		}
		else if(0 == state)
		{
			printf("客户端关闭链接\n");
			break;
		}
		else
		{
			if(sel == 3)
				break;
			switch(sel)
			{
			case 1:

				login(clnt_fd, db);
				//		printf("login被执行了一次\n");
				break;
			case 2:
				logon(clnt_fd, db);
				break;
			default:
				break;
			}
		}
	}

	//printf("usr_name=%s\n",login.usr_name);
	close(clnt_fd);
	sqlite3_close(db);

	pthread_exit(NULL);
}
int login(int clnt_fd, sqlite3 *db)
{

	usr_info login;//存储用户信息
	sevrpack sendpack;
	char *errmsg;
	char commond[512];
	char **resultp;
	int nrow, ncolumn;
	//printf("在这里等待\n");
	int try_num;
	int state;
	for(try_num = 0; try_num < 3; try_num++)//尝试三次登录
	{
		state = recv(clnt_fd, (void *)(&login), sizeof(usr_info), 0);
		if(state < 0)
		{
			printf("与客户端连接出错\n");
			break;
		}
		else if(0 == state)
		{
			printf("客户端关闭链接\n");
			break;
		}
		else
		{
			bzero(commond, sizeof(commond));
			sprintf(commond, "select * from usr where name = '%s'", login.usr_name);
			if(sqlite3_get_table(db, commond, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
			{
				printf("%s\n",errmsg);
				return -1;
			}
			if(nrow == 0)//没有这个用户
			{
				//printf("if 执行了一次\n");
				bzero((void*)&sendpack, sizeof(sevrpack));
				//printf("设置type 前执行了一次\n");
				sendpack.type =ABSENT;
				//printf("strcp 前执行了一次\n");
				strcpy(sendpack.msg, "user is noexists");
				//printf("send前执行了一次\n");
				send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);
				//printf("执行了一次\n");
				//printf("%d\n",sendpack.type);

			}
			else //有用户名，继续校验密码
			{
				if(0 == strcmp(login.password, resultp[3]))
				{
					bzero((void*)&sendpack, sizeof(sevrpack));
					sendpack.type =CHECK_SUCCESS;
					//strcpy(sendpack.msg, "password is failed");
					send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);
					search_sys(clnt_fd, db);
					break;
				}
				else
				{
					bzero((void*)&sendpack, sizeof(sevrpack));
					sendpack.type =CHECK_FAIL;
					strcpy(sendpack.msg, "password is failed");
					send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);
				}
				/*	bzero(commond, sizeof(commond));
					sprintf(commond, "select * from usr where password = '%s'", login.password);
					if(sqlite3_get_table(db, commond, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
					{
					printf("%s\n",errmsg);
					return -1;
					}
					if(nrow == 0)
					{
					bzero((void*)&sendpack, sizeof(sevrpack));
					sendpack.type =CHECK_FAIL;
					strcpy(sendpack.msg, "password is failed");
					send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);
					}
					else//校验成功
					{
					bzero((void*)&sendpack, sizeof(sevrpack));
					sendpack.type =CHECK_SUCCESS;
				//strcpy(sendpack.msg, "password is failed");
				send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);
				search_sys(clnt_fd, db);
				break;

				}*/

			}
		}
	}
	sqlite3_free_table(resultp);
}


int logon(int clnt_fd, sqlite3 *db)
{
	usr_info logon;
	sevrpack sendpack;
	char *errmsg;
	char commond[512];
	char **resultp;
	int nrow, ncolumn;
	//printf("在这里等待\n");
	recv(clnt_fd, (void *)(&logon), sizeof(usr_info), 0);

	bzero(commond, sizeof(commond));
	sprintf(commond, "select * from usr where name = '%s'", logon.usr_name);
	if(sqlite3_get_table(db, commond, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	if(nrow == 0)//没有这个用户名可以注册
	{
		bzero(commond, sizeof(commond));
		sprintf(commond, "insert into usr values('%s','%s')", logon.usr_name, logon.password);
		if(sqlite3_exec(db, commond, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("error:%s\n",errmsg);
			return -1;
		}
		bzero((void*)&sendpack, sizeof(sevrpack));
		sendpack.type =CHECK_SUCCESS;
		send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);
	}
	else
	{
		bzero((void*)&sendpack, sizeof(sevrpack));
		sendpack.type =ABSENT;
		strcpy(sendpack.msg, "user already exists");
		send(clnt_fd, (void *)&sendpack, sizeof(sevrpack), 0);

	}


	sqlite3_free_table(resultp);
}
int search_sys(int clnt_fd,sqlite3 *db)
{
	printf("已经进入查询系统\n");
	int sel;
	char word[32];
	char commond[512];
	char *errmsg;
	char **resultp;
	int nrow, ncolumn;
	translate sevr_send;
	int i,j;
	while(1)
	{
		sel = 0;
		recv(clnt_fd, (void *)&sel, sizeof(int), 0);
		switch(sel)
		{
		case 1:
			bzero(word, sizeof(word));
			recv(clnt_fd, (void *)word, sizeof(word), 0);
			printf("要查询的词是%s\n",word);
			bzero(commond, sizeof(commond));
			sprintf(commond, "select * from word where word = '%s'", word);
			printf("%s\n", commond);
			if(sqlite3_get_table(db, commond, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
			{
				printf("%s\n",errmsg);
				return -1;
			}
			printf("已经获取了数据库的查询结果\n");
			//		printf("%d\n",nrow);
			//		printf("%s\t%s\n",resultp[2],resultp[3]);

			if(nrow == 1)
			{
				sevr_send.type = CHECK_SUCCESS;
				sprintf(sevr_send.word, resultp[2]);
				sprintf(sevr_send.mean, resultp[3]);

				send(clnt_fd, (void *)&sevr_send, sizeof(sevr_send), 0);
				printf("已经发送完查询结果\n");
				//printf("%d\n",strlen(*resultp[2]));
				/*将查询的词计入查询历史数据库*/
				into_hist(db, resultp[2]);

			}
			else
			{
				sevr_send.type = ABSENT;
				send(clnt_fd, (void *)&sevr_send, sizeof(sevr_send), 0);
			}
			break;
		case 2:
			bzero(commond, sizeof(commond));
			//bzero(resultp, sizeof(resultp));
			sprintf(commond, "select * from queryhistory");
			if(sqlite3_get_table(db, commond, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
			{
				printf("%s\n",errmsg);
				return -1;
			}
			int index = ncolumn;
			printf("nrow = %d\n",nrow);
			send(clnt_fd, (void *)&nrow, sizeof(int), 0);//发送查询到的数量发给客户端，用以控制循环接收的循环次数
			for(i = 0; i < nrow; i++)
			{
				sprintf(word, resultp[index+1]);
				send(clnt_fd, (void *)word, sizeof(word), 0);
				index += ncolumn;
			}
			//send(clnt_fd, (void *)word, sizeof(word), 0);

			//历史记录
			break;
		case 3:
			//个人信息
			break;
		default:
			return 0;	
		}
	}
	printf("已经跳出while循环\n");
	sqlite3_free_table(resultp);

}

int into_hist(sqlite3 *db, char *resul)
{
	char commond[512];
	char *errmsg;
	char **resultp;
	int nrow, ncolumn;
	bzero(commond, sizeof(commond));
	sprintf(commond, "insert into queryhistory(word) values('%s')", resul);
	if(sqlite3_exec(db, commond, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("error:%s\n",errmsg);
		return -1;
	}
	/*检查记录条数*/
	bzero(commond, sizeof(commond));
	sprintf(commond, "select * from queryhistory");
	if(sqlite3_get_table(db, commond, &resultp, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	printf("%d\n", nrow);

	if(nrow > 20)
	{
		bzero(commond, sizeof(commond));
		sprintf(commond, "delete from queryhistory where word='%s'",resultp[3]);
		printf("%s\n",commond);
		if(sqlite3_exec(db, commond, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("%s\n",errmsg);
			return -1;
		}

	}

	sqlite3_free_table(resultp);
}

int main()
{
	int lisn_fd;//监听套接字描述符
	struct sockaddr_in my_addr;
	//bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(6666);
	//my_addr.sin_addr.s_addr = inet_addr("192.168.1.117");
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t addrlen;
	addrlen = sizeof(my_addr);
	int clnt_fd;//与客户端通信的描述符
	pthread_t clnt_thread;
	pthread_attr_t attr;

	/*初始化线程属性对象*/
	if(pthread_attr_init(&attr) != 0)
	{
		perror("create attribute failed");
		return -1;
	}
	/*设置线程分离属性*/
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
	{
		perror("setdatachstate failed");
		return -1;
	}



	if((lisn_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket:");
		return -1;
	}
	printf("lisn_fd=%d\n",lisn_fd);

	if(bind(lisn_fd, (struct sockaddr *)&my_addr, addrlen) == -1)
	{
		perror("bind:");
		return -1;
	}
	if(listen(lisn_fd, 10) == -1)
	{
		perror("listen:");
		return -1;
	}
	while(1)
	{
		if((clnt_fd = accept(lisn_fd, (struct sockaddr *)&my_addr, &addrlen)) == -1)
		{
			perror("accept:");
			return -1;
		}
		printf("clnt_fd=%d\n",clnt_fd);
		if(pthread_create(&clnt_thread, &attr, thrd_func,(void *)clnt_fd) != 0)
		{
			perror("create thread failed");
			return -1;
		}
		/*释放线程属性对象*/
		pthread_attr_destroy(&attr);
	}


	return 0;
}
