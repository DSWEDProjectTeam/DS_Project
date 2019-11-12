#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAXSIZE 100

int main(int argc,char* argv[]) {
    int server_sock;
    int connect_sock;
    int addrlen, datalen;

    char buf[MAXSIZE +1];
    int nbytes;

    struct sockaddr_in server_addr;
    struct sockaddr_in connect_addr;

    pid_t pid;

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

	pid = fork();

    if(pid == -1) {
	close(connect_sock);
	perror("fork() error");
	exit(-1);
	}
    else if (pid == 0) { 
        while(1) {
            fgets(buf,sizeof(buf),stdin);
            nbytes = strlen(buf);
            write(connect_sock,buf,MAXSIZE);
            if((strncmp,"exit",4) == 0){
                puts("Exit Program.");
                exit(-1);
            } 
        }    
        exit(-1);
    }

    else if(pid > 0) { 
        while(1) {
            if((nbytes = read(connect_sock,buf,MAXSIZE)) <0) {
                perror("read() error\n");
                exit(-1);
            }
            printf("%s",buf);
            if(strncmp(buf,"exit",4) == 0)
                exit(-1);
            } 
        }    
    return 0;
}



