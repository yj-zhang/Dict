#include <client.h>


int main(int argc, char *argv[])
{
    int sockfd;
    cmd_t cmd;
    unsigned char lo_data[sizeof(struct XProtocol)];

    /*初始化*/
    if ((sockfd = init_config()) <= 0){
        printf("initialize configuration error\n");
        return -1;
    }

    /*打印菜单*/

    while (1) 
	{
        if (ustate == ONLINE)//表示客户端有没有登陆成功的
		{
            dis_menus();
            /*获取命令*/
            if ((cmd = get_cmds()) > FMAX)
			{
                printf("error>I'm afraid I don't know what you want me to do\n");
                continue;
            }
            /*功能选择*/
            if (sec_fun(cmd, sockfd, lo_data) != 0){
                printf("some error in sec_fun\n");
                return -1;
            }
        } else {
            dis_menul();
            /*获取命令*/
            if ((cmd = get_cmdl()) > FMAX)
			{
                printf("error>I'm afraid I don't know what you want me to do\n");
                continue;
            }
            /*功能选择*/
            if (sec_fun(cmd, sockfd, lo_data) != 0)
			{
                printf("some error in sec_fun\n");
                return -1;
            }
        }
    }

    return 0;
}

/*初始化配置*/
int init_config()
{   
    int sockfd;
    struct sockaddr_in servaddr;

    /*用户标识码*/
    srand(time(NULL));
    ukey = rand();
    /*用户为未登陆状态*/
    ustate = OFFLINE;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        perror("fail to socket");
        exit(-1);
    }
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("connect");
        exit(-1);
    }
    return sockfd;
}

/*打印登陆菜单*/
void dis_menul()
{
    printf("\t1.register(注册);\n");
    printf("\t2.login(登陆);\n");
    printf("\t3.help(帮助);\n");
    printf("\t0.quit(退出);\n");
    printf(">");
}

/*打印查询菜单*/
void dis_menus()
{
    printf("\t1.look up words(查单词);\n");
    printf("\t2.list history(查看历史单词);\n");
    printf("\t3.help(帮助);\n");
    printf("\t0.quit(退出);\n");
    printf(">");
}

/*打印帮助*/
void dis_help()
{
    printf("help\n");    
}

/*获取用户输入命令*/
cmd_t get_cmdl()
{
    char buff[10];
    fgets(buff, 10, stdin);
#ifdef _DEBUG
    //printf("get_cmd = %s",buff);
#endif
    if (!strncmp(buff,"reg", 3) || !strncmp(buff, "1", 1)){
        return (cmd_t)FREGISTER;
    } else if (!strncmp(buff,"log", 3) || !strncmp(buff, "2", 1)){
        return (cmd_t)FLOGIN;
    } else if (!strncmp(buff,"hel", 3) || !strncmp(buff, "3", 1)){
        return (cmd_t)FDIS_HELP;
    } else if (!strncmp(buff,"q", 1) || !strncmp(buff, "0", 1)){
        return (cmd_t)FQUIT;
    } else if (!strncmp(buff,"\n", 1)){
        return (cmd_t)FDIS_MENUL;
    }
    return -1;
}

/*获取用户输入命令*/
cmd_t get_cmds()
{
    char buff[10];
    fgets(buff, 10, stdin);
#ifdef _DEBUG
    printf("get_cmd = %s",buff);
#endif
    if (!strncmp(buff,"loo", 3) || !strncmp(buff, "1", 1)){
        return (cmd_t)FSEARCH;
    } else if (!strncmp(buff,"lis", 3) || !strncmp(buff, "2", 1)){
        return (cmd_t)FLIST_HIS;
    } else if (!strncmp(buff,"hel", 3) || !strncmp(buff, "3", 1)){
        return (cmd_t)FDIS_HELP;
    } else if (!strncmp(buff,"q", 1) || !strncmp(buff, "0", 1)){
        return (cmd_t)FQUIT;
    } else if (!strncmp(buff,"\n", 1)){
        return (cmd_t)FDIS_MENUS;
    }
    return -1;
}

/*选择功能*/
int sec_fun(cmd_t cmd, int sockfd, unsigned char *lo_data)
{
    int retval = 0;

    switch (cmd)
	{
        case FREGISTER:
            retval = fregister(sockfd,lo_data);
            break;
        case FLOGIN:
            retval = flogin(sockfd, lo_data);
            break;
        case FDIS_HELP:
            dis_help();
            break;
        case FSEARCH:
            fsearch(sockfd, lo_data);
            break;
        case FLIST_HIS:
            flist_his(sockfd, lo_data);
            break;
        case FDIS_MENUL:
            dis_menul();
            break;
        case FDIS_MENUS:
            dis_menus();
            break;
        case FQUIT:
            retval = quit();
            break;
        default:
            retval = -1;
            break;
    }
#ifdef _DEBUG_
    printf("sec_fun cmd = %d\n",cmd);
#endif
    return retval;
}

/*退出*/
int quit()
{
#ifdef _DEBUG
    printf("quit\n");
#endif
    exit(0);
}
