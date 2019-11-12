#include<unistd.h>
#include<arpa/inet.h> 
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define MAXSIZE 100

int main(int argc, char *argv[]) {
	int client_sock;
	struct sockaddr_in serverinfo;
	int datalen;
	pid_t pid;
	
	char buf[MAXSIZE + 1];


	if (argc != 3) {
		printf("Usage : %s <IP> <Port> \n", argv[0]);
		exit(1);
	}

	client_sock = socket(PF_INET, SOCK_STREAM, 0); 

	if (client_sock == -1) {
		perror("socket() failed\n");
		exit(1);
	}

	memset(&serverinfo, 0, sizeof(serverinfo);
	serverinfo.sin_family = AF_INET;
	serverinfo.sin_addr.s_addr = inet_addr(argv[1]); 
	serverinfo.sin_port = htons(atoi(argv[2])); 

	if (-1 == connect(client_sock, (struct sockaddr*)&serverinfo, sizeof(serverinfo))) 
	{ 
		perror("connect() error\n");
		exit(1);
	}
	
	switch (pid = fork()) {
	case -1: 
			perror("fork() error\n");
				exit(0);
		
	case 0: 
			while (1) {       
				fgets(buf, sizeof(buf), stdin);
					
					write(client_sock, buf, MAXSIZE);
					if ((strncmp, "exit", 4) == 0) {
						puts("Exit Program.");
							exit(0);
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
				{
					exit(1);
				}
			}
		}
	
	close(client_sock);
	return 0;
}
