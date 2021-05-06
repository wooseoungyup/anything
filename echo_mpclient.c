#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define BUF_SIZE 1024
void error_handling(char* message);

//읽기 처리 (입력)
void read_routine(int sock, char* buf);

//쓰기 처리 (출력)
void write_routine(int sock, char* buf);

int main(int argc, char* argv[])
{

    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;


    //실행경로 + ip + port 인자로 받는다.
    if (argc != 3) {
        printf("usage :  %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    //TCP 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        error_handling("socket() error");
    }

    //서버 주소 정보 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);  //ip
    serv_adr.sin_port = htons(atoi(argv[2]));       //port



    //서버 주소정보를 기반으로 연결 요청
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        error_handling("connect() error");
    }
    else {

        puts("connected.....");

        pid = fork();   //자식 프로세스 생성
        if (pid == 0) {
            write_routine(sock, buf); //자식 프로세스 - 출력 루틴처리
        }
        else {
            read_routine(sock, buf);  //부모 프로세스 - 입력 루틴처리
        }



    }

    close(sock);
    return 0;
}

//입력받기 함수
void read_routine(int sock, char* buf) {

    while (true)
    {

        int str_len = read(sock, buf, BUF_SIZE - 1); //서버로 부터 데이터 수신
        //printf("read_routine - str_len : %d \n" , str_len);

        //서버에서 읽은게 없으면 종료
        if (str_len == 0) {
            printf("str_len == 0 \n");
            return;
        }

        buf[str_len] = '\0';
        printf("message from server : %s \n", buf);

    }
}

//출력하기 함수
void write_routine(int sock, char* buf) {

    while (1)
    {
        fputs("Input message(Q to quit) : ", stdout);
        fgets(buf, BUF_SIZE, stdin); //표준입력으로 부터 입력 받음

        //종료 키를 누르면
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            shutdown(sock, SHUT_WR); //출력 스트림 종료
            return;
        }

        //널문자를 제외한 문자열을 서버로 송신
        write(sock, buf, strlen(buf));
        // printf("write - 서버로 출력 완료. \n" );
    }
}


//에러 처리
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}