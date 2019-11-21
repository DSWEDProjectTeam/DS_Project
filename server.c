#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

#define MAX_INFO 30
#define MAXSIZE 100
#define MAX_NUM 100

struct __User {
	char id[MAX_INFO + 1];
	char passwd[MAX_INFO + 1];
};
typedef struct __User User;


int openFile(User *ptr, int* num);
int insert(User* ptr, int* num);
int logIn(User* ptr, int* num);

int main(int argc,char* argv[]) {
    int server_sock;
    int connect_sock;
    int addrlen, datalen;

    char buf[MAXSIZE + 1];
    User user[MAX_INFO];
	int person = 0;

    struct sockaddr_in server_addr;
    struct sockaddr_in connect_addr;

    pid_t pid;

    int fd;
	int input=0;

    if(argc != 2) {
        printf("Usage : %s <port>\n",argv[0]);
        exit(-1);
    }

    server_sock = socket(PF_INET,SOCK_STREAM, IPPROTO_TCP);

    if(server_sock == -1) {
        perror("socket() error\n");
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_sock, (struct sockaddr*)&server_addr,sizeof(server_addr)) == -1) {
        perror("bind() error\n");
        exit(-1);
    }

    if(listen(server_sock,1) == -1) {
        perror("listen() error\n");
        exit(-1);
    }

    addrlen = sizeof(connect_sock);
    connect_sock = accept(server_sock, (struct sockaddr*)&connect_addr, &addrlen);

	if(connect_sock == -1) {
        perror("accept() error\n");
        exit(-1);
    }
    else
    printf("Client Connected!\n");
    openFile(user,&person);

	while(1) {
		END:
		printf(" ============================\n");
		printf("|                            |\n");
		printf("|  1. Sign  Up               |\n");
		printf("|  2. Login  &  Start  chat  |\n");
		printf("|  3. Sign  Out              |\n");
		printf("|  4. Exit                   |\n");
		printf("|                            |\n");
		printf(" ============================\n\n");
		printf(" Input Option: ");
		scanf("%d", &input);

		if(input == 1)
			insert(user, &person);
		else if(input == 2)
		{
			if(logIn(user, &person))
			{
				while(1) {
					switch(pid = fork()) {
                    case -1:
							perror("fork() error\n");
							exit(0);
                    case 0:
							while (1) {
							fgets(buf, sizeof(buf), stdin);
							printf("Message (q to exit): ");
							write(connect_sock, buf, MAXSIZE);
							if(strncmp(buf, "q", 1)==0) {
								puts("Exit Chatting !!\n\n");
								goto END;
							}
							}
							exit(1);

							default:
							while (1) {
								if (read(connect_sock, buf, MAXSIZE) < 0) {
								perror("read() error\n");
								exit(0);
							}
							printf("%s", buf);
							if (strncmp(buf, "exit", 4) == 0)
								exit(1);
							}
        			}
				}
			}
		}
		else if(input == 4)
		{
			printf(" Exit! Chatting Program !!\n");
			return 0;
		}
		else
		{
			printf("Please enter only 1,2,3,4 !!\n\n");
			input =0;
		}
	}
	close(server_sock);
    return 0;
}



int openFile(User* ptr, int*num) {
	int state;
	char temp;
	FILE* fp = fopen("UserInfo.txt", "rt");

	if(fp == NULL)
	{
		perror("open() error!");
		return 1;
	}
	while(1)
	{
		fscanf(fp, "%s %s", ptr[*num].id, ptr[*num].passwd);
		if(feof(fp)!=0)
			break;
			(*num)++;
	}
	state=fclose(fp);
	if(state!=0) {
		printf("file close error\n");
		return 1;
	}
	return 0;
}

int insert(User* ptr, int* num){

    if (*num < MAX_NUM){
		printf(" ============================\n");
		printf("|\n");
        printf("| Enter your ID :");
        scanf("%s", ptr[*num].id);
        printf("| Enter your Password:");
        scanf("%s", ptr[*num].passwd);
		printf("|\n");
        (*num)++;
        printf("| Data Inserted              |\n");
    }
    else
        printf(" =======================================\n");
   		printf("| ERROR :  Data Full !!                 |\n");
		printf(" =======================================\n");

	if(*num>0) {
		int i, state;
		FILE*fp = fopen("UserInfo.txt","wt");

		if(fp == NULL) {
			printf("open ()error!");
			return 1;
		}

		for(i=0;i<*num;i++) {
			fprintf(fp, "%s %s", ptr[i].id, ptr[i].passwd);
			fputc('\n', fp);
		}
		state = fclose(fp);
		if(state!=0) {
			printf("file close() error\n");
			return 1;
		}

		printf("| Information saved          |\n");
		printf("|\n");
		printf(" ============================\n\n\n\n");
		return 0;
	}
}


int logIn(User* ptr, int* num)
{
    char id[30];
	char passwd[30];
    int i;

    if (*num > 0){
		printf("\n");
		printf(" =======================================\n");
        printf("| ID: ");
        scanf("%s", id);
		printf("| PASSWORD: ");
		scanf("%s", passwd);
		printf(" =======================================\n");
		printf("\n");
		for (i = 0; i < MAX_NUM; i++){
            if (!strcmp(id, ptr[i].id) && !strcmp(passwd, ptr[i].passwd)){
				printf(" =======================================\n");
                printf("| Login Successful!!  Start Chatting !! |\n");
				printf(" =======================================\n\n");
                return 1;
            }
        }
    printf(" =======================================\n");
    printf("| LogIn Fail :  Wrong ID or PASSWORD !! |\n");
	printf(" =======================================\n");
    return 0;
    }
    else{
        printf(" =======================================\n");
    	printf("| LogIn Fail :  No Data !!              |\n");
		printf(" =======================================\n");
        return 0;
 	}
}