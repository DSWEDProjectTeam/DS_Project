#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_INFO 30
#define MAXSIZE 100
#define MAX_NUM 100

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_BRIGHT_BLUE "\x1b[36m"

struct __User 
{
    char id[MAX_INFO + 1];
    char passwd[MAX_INFO + 1];
};
typedef struct __User User;

int openFile(User *ptr, int *num);
int insert(User *ptr, int *num);
int logIn(User *ptr, int *num, char *id, char *passwd);
int deleted(User *ptr, int *num);

int main(int argc, char *argv[]) 
{
    int client_sock;
    struct sockaddr_in serverinfo;
    int datalen;
    pid_t pid;

    char buf[MAXSIZE + 1];
    char data[MAXSIZE + 1];
    char id[30];
    char passwd[30];
    User user[MAX_INFO];
    int person = 0;
    char *input = malloc(sizeof(char) * 10);

    if (argc != 3) 
    {
        printf("Usage : %s <IP> <Port> \n", argv[0]);
        exit(1);
    }

    client_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (client_sock == -1) 
    {
        perror("socket() failed\n");
        exit(1);
    }

    memset(&serverinfo, 0, sizeof(serverinfo));
    serverinfo.sin_family = AF_INET;
    serverinfo.sin_addr.s_addr = inet_addr(argv[1]);
    serverinfo.sin_port = htons(atoi(argv[2]));

    if (-1 == connect(client_sock, (struct sockaddr *)&serverinfo,
                      sizeof(serverinfo))) 
                      {
        perror("connect() error\n");
        exit(1);
    }
    openFile(user, &person);

    while (1) 
    {
    END:
        system("clear");
        printf(COLOR_YELLOW);
        printf(" __      ____  _____   _   ____ \n");
        printf("|  |    |  __||_   _| |_| /  __)\n");
        printf("|  |    | |_    | |   _|| | (_  \n");
        printf("|  |    |  _|   | |       \\_  \\\n");
        printf("|  |___ | |__   | |       __) |\n");
        printf("|______||____|  |_|      (____/ \n");
        printf("                                \n");
        printf("        _               _     _ \n");
        printf("  ____ | |      ____  _| |_  | | \n");
        printf(" /  __|| |___  / _  ||_   _| |_| \n");
        printf(" | |__ |  _  || (_| |  | |_   _   \n");
        printf(" \\____||_| |_| \\__,_|   \\__| |_| \n");
        printf("                                \n");

        printf(COLOR_BRIGHT_BLUE);
        printf(" ============================== \n");
        printf("|                              |\n");
        printf("|  1. Sign  Up                 |\n");
        printf("|  2. Login  &  Start  chat    |\n");
        printf("|  3. Sign  Out                |\n");
        printf("|  4. Exit                     |\n");
        printf("|                              |\n");
        printf(" ==============================\n\n");

        printf(COLOR_GREEN);
        printf("  Input Option: ");

        scanf("%s", input);
        int dasin = atoi(input);

        if (dasin == 1) 
        {
            system("clear");
            insert(user, &person);
        } 
        else if (dasin == 2) 
        {
            system("clear");
            printf("\n");
            printf(" =======================================\n");
            printf("| ID: ");
            scanf("%s", id);
            printf("| PASSWORD: ");
            scanf("%s", passwd);
            printf(" =======================================\n");
            printf("\n");
            if (logIn(user, &person, id, passwd)) {
                switch (pid = fork()) {
                case -1:
                    perror("fork() error\n");
                    exit(0);

                case 0:
                    printf("Input EXIT to exit !!");
                    printf("\n");
                    while (1) 
                    {
                        fgets(buf, sizeof(buf), stdin);
                        sprintf(data, "%s : %s", id, buf);
                        write(client_sock, data, MAXSIZE);
                        if (strncmp(buf, "EXIT", 4) == 0) 
                        {
                            puts("Exit Chatting !!\n\n");
                            goto END;
                        }
                    }
                    exit(1);

                default:
                    while (1) 
                    {
                        if (read(client_sock, buf, MAXSIZE) < 0) 
                        {
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
        else if (dasin == 3) 
        {
            deleted(user, &person);
        } 
        else if (dasin == 4)
        {
            printf(" Exit! Chatting Program !!\n\n\n");
            return 0;
        } 
        else 
        {
            system("clear");
				printf(COLOR_RED);
				printf("===============================\n");
            printf("| please enter only 1,2,3,4 !! |\n");
				printf("===============================\n\n");
            sleep(1);
        }
    }
    free(input);
    close(client_sock);
    return 0;
}

int openFile(User *ptr, int *num) 
{
    int state;
    char temp;
    FILE *fp = fopen("UserInfo.txt", "rt");

    if (fp == NULL) 
    {
        perror("open() error!");
        return 1;
    }
    while (1) 
    {
        fscanf(fp, "%s %s", ptr[*num].id, ptr[*num].passwd);
        if (feof(fp) != 0)
            break;
        (*num)++;
    }
    state = fclose(fp);
    if (state != 0) 
    {
        printf("file close error\n");
        return 1;
    }
    return 0;
}

int insert(User *ptr, int *num) 
{
    char id[30];
    char passwd[30];
    if (*num < MAX_NUM) 
    {
        printf(COLOR_YELLOW);
        printf(" ============================\n");
        printf("|\n");
        printf("| Enter ID :");
        scanf("%s", id);
        printf("| Enter Password:");
        scanf("%s", passwd);
        system("clear");
        printf(" =======================================\n");
        printf("| SAVING ###########                    |\n");
        printf(" =======================================\n");
        sleep(1);
        system("clear");
        printf(" =======================================\n");
        printf("| SAVING #####################          |\n");
        printf(" =======================================\n");
        sleep(1);
        system("clear");
        printf(" =======================================\n");
        printf("| SAVING ###############################|\n");
        printf(" =======================================\n");

        for (int i = 0; i < MAX_NUM; i++) {

            if (!strcmp(id, ptr[i].id)) 
            {
                sleep(1);
                system("clear");
                printf(COLOR_RED);
                printf(" =======================================\n");
                printf("| ERROR :  ID already exists !!         |\n");
                printf(" =======================================\n");
                sleep(1);
                return 0;
            }
        }
        strcpy(ptr[*num].id, id);
        strcpy(ptr[*num].passwd, passwd);
        (*num)++;
        sleep(1);

    } else if (*num = MAX_NUM) 
    {
        sleep(1);
        system("clear");
        printf(COLOR_RED);
        printf(" =======================================\n");
        printf("| ERROR :  Data Full !!                 |\n");
        printf(" =======================================\n");
        sleep(1);
    }

    if (*num > 0) 
    {
        int i, state;
        FILE *fp = fopen("UserInfo.txt", "wt");

        if (fp == NULL) 
        {
            printf("open ()error!");
            return 1;
        }

        for (i = 0; i < *num; i++) 
        {
            fprintf(fp, "%s %s", ptr[i].id, ptr[i].passwd);
            fputc('\n', fp);
        }
        state = fclose(fp);
        if (state != 0) 
        {
            printf("file close() error\n");
            return 1;
        }
        system("clear");
        printf(" =======================================\n");
        printf("|  Information Saved !!!                |\n");
        printf(" =======================================\n");
        sleep(1);
        return 0;
    }
}

int logIn(User *ptr, int *num, char *id, char *passwd) 
{
    int i;

    if (*num > 0) 
    {
        for (i = 0; i < MAX_NUM; i++) 
        {
            if (!strcmp(id, ptr[i].id) && !strcmp(passwd, ptr[i].passwd)) 
            {
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
    else 
    {
        printf(" =======================================\n");
        printf("| LogIn Fail :  No Data !!              |\n");
        printf(" =======================================\n");
        return 0;
    }
}

int deleted(User *ptr, int *num) 
{
    char id[30];
    char passwd[30];
    int i, j;

    if (*num > 0) 
    {
		  printf(COLOR_RED);
		  printf("========================================\n");
		  printf("| Your ID cannot be reused or recovered |\n");
		  printf("| If you leave. Withdrawal ID can't be  |\n");
		  printf("| reused or restored by both  yourself  |\n");
		  printf("| and others. Please choose carefully!  |\n");
		  printf("|                                       |\n");
		  printf("| If you still want to delete,          |\n");
		  printf("| please enter your Id  Passwd below.   |\n");
        printf("| Input ID:  ");
        scanf("%s", id);
        printf("| Input passwd:  ");
        scanf("%s", passwd);
		  printf("|\n");
		  printf("=======================================\n\n\n");

        for (i = 0; i < MAX_NUM; i++) 
        {
            if (strcmp(id, ptr[i].id) == 0) 
            {
                (*num)--;
					 printf(COLOR_BLUE);
					 system("clear");
					 printf("=======================================\n");
					 printf("| Delete ########                      |\n");
					 printf("=======================================\n");
					 sleep(1);
					 system("clear");
					 printf("=======================================\n");
					 printf("| Delete ###################           |\n");
					 printf("=======================================\n");
					 sleep(1);
					 system("clear");
					 printf("=======================================\n");
					 printf("| Delete ##############################|\n");
					 printf("=======================================\n");
					 sleep(1);
					 system("clear");
					 printf("=======================================\n");
                printf("|    Member Withdrawal Completed!!!    |\n");
					 printf("=======================================\n\n\n");
                if (i != MAX_NUM - 1) {
                    for (j = i; j < MAX_NUM; j++) 
                    {
                        strcpy(ptr[j].id, ptr[j + 1].id);
                        strcpy(ptr[j].passwd, ptr[j + 1].passwd);
                    }

                    *ptr[MAX_NUM - 1].id = 0;
                    *ptr[MAX_NUM - 1].passwd = 0;
                } 

                else 
                {
                    *ptr[MAX_NUM - 1].id = 0;
                    *ptr[MAX_NUM - 1].passwd = 0;
                }
                return 0;
            }
        }
		  printf(COLOR_RED);
		  printf("=======================================\n");
        printf("|   No such information was found!!!  |\n");
		  printf("=======================================\n\n\n");
        return 0;
    } 
    
    else 
    {
		  printf(COLOR_RED);
		  printf("========================================\n");
        printf("|   There is no search information!!!   |\n");
		  printf("========================================\n\n\n");
        return 0;
    }
}
