#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>

#define MAX_INFO 30
#define MAXSIZE 100
#define MAX_NUM 100

struct __User {
	char id[MAX_INFO + 1];
	char passwd[MAX_INFO + 1];
};
typedef struct __User User;

char username[30] = {};

int openFile(User *ptr, int* num);
int insert(User* ptr, int* num);
int logIn(User* ptr, int* num);

int main(int argc, char *argv[]) {
	int client_sock;
	struct sockaddr_in serverinfo;
	int datalen;
	pid_t pid;

	char buf[MAXSIZE + 1];
	User user[MAX_INFO];
	int person = 0;


	if (argc != 3) {
		printf("Usage : %s <IP> <Port> \n", argv[0]);
		exit(1);
	}

	client_sock = socket(PF_INET, SOCK_STREAM, 0);

	if (client_sock == -1) {
		perror("socket() failed\n");
		exit(1);
	}

	memset(&serverinfo, 0, sizeof(serverinfo));
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_addr.s_addr = inet_addr(argv[1]);
	serverinfo.sin_port = htons(atoi(argv[2]));

	if (-1 == connect(client_sock, (struct sockaddr*)&serverinfo, sizeof(serverinfo)))
	{
		perror("connect() error\n");
		exit(1);
	}
	openFile(user,&person);

	while(1) {
				printf("====================\n\n");
				printf("1. Sign Up\n");
				printf("2. Login & Start chat\n");
				printf("3. Exit\n\n");
				printf("====================\n\n");
				int input=0;
				scanf("%d", &input);
				if(input == 1)
					insert(user, &person);
				else if(input == 2)
					if(logIn(user, &person)) {

						switch (pid = fork()) {
							case -1:
								perror("fork() error\n");
								exit(0);

							case 0:
								while (1) {
								printf("%s: ", username);
								fgets(buf, sizeof(buf), stdin);

								write(client_sock, buf, MAXSIZE);
								if ((strncmp, "exit", 4) == 0) {
									puts("Exit Program.");
									return 0;
								}
								}
							exit(1);

							default:
							while (1) {
								if (read(client_sock, buf, MAXSIZE) < 0) {
								perror("read() error\n");
								exit(0);
							}
							printf("%s", buf);
							if (strncmp(buf, "exit", 4) == 0)
								exit(1);
							}
						}
					}
				else if(input == 4)
				{
					printf("exit!");
					return 1;
				}
				else
				{
					printf("please enter only 1,2,3 !!");
					input =0;
				}
	}

	close(client_sock);
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
          printf("Input id: ");
          scanf("%s", ptr[*num].id);
          printf("Input Password: ");
          scanf("%s", ptr[*num].passwd);

          (*num)++;
          printf("\nData Inserted\n");
     }
     else
          printf("  Data Full \n\n");

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
			printf("file close error\n");
			return 1;
		}

		printf("Information saved\n\n");
		return 0;
	}
}


int logIn(User* ptr, int* num)
{
    char id[30];
	char passwd[30];
    int i;

    if (*num > 0){
        printf("Input id: ");
        scanf("%s", id);
		printf("Input passwd: ");
		scanf("%s", passwd);
        for (i = 0; i < MAX_NUM; i++){
            if (!strcmp(id, ptr[i].id) && !strcmp(passwd, ptr[i].passwd)){
                printf("Login Successful!!\nStart Chatting !!\n");
				strcpy(username, id);
                return 1;
            }
        }
    printf("ID Not Found \n\n");
    return 0;
    }
    else{
        printf("No Data \n\n");
        return 0;
 	}
}
