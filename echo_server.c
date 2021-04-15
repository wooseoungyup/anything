#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(char* message);

int main(int argc, char* argv[]) {

    //서버, 클라이언트 소켓 디스크립터
    int serv_sock, clnt_sock;
    //메시지 버퍼
    char message[BUF_SIZE];

    int str_len, i;

    //주소 정보 구조체
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    //터미널 인자개수 체크
    if (argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    //소켓 생성 : ipv4 ,tcp
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handle("socket() error");
    }
    else {
        printf("socket success ! \n");
    }

    //주소 설정을 위한 메모리 0으로 초기화, ip, port 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    //바인딩 - 주소 셋팅
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handle("bind error");
    }
    else {
        printf("bind success ! \n");
    }

    //대기 큐 5개 생성
    if (listen(serv_sock, 5) == -1) {
        error_handle("listen error");
    }
    else {
        printf("listen success ! \n");
    }

    //클라이언트 주소설정 길이
    clnt_adr_sz = sizeof(clnt_adr);

    //클라이언트는 5번 또는 5개의 클라이언트만 접속 가능하다.
    for (i = 0; i < 5; i++) {

        printf("Entering for-loop i : %d \n", i);

        //접속 - 클라이언트 연결 대기중....
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        if (clnt_sock == -1) {
            error_handle("accept error");
        }
        else {
            printf("accept success \nconnected client %d \n", i + 1);
        }


        //서버에서 클라이언트가 보낸 메시지 읽어서 다시 클라이언트로 전송 처리
        while (1) {

            //#.문제
            //하나의 클라이언트가 쓴것만 동시에 읽는 구조... (여러 클라이언트를 처리할 수 없다..)
            //while 에서 계속 맴돌고,
            //클라이언트가 q를 누르고 종료가 되어야.. 그다음 클라이언트 접속이 처리된다....
            str_len = read(clnt_sock, message, BUF_SIZE);

            //클라이언트에서 보낸 문자 바이트 수
            printf("str_len : %d \n", str_len);

            if (str_len != 0) {
                write(clnt_sock, message, str_len);
            }
            else {
                printf("break \n");
                break;
            }

        }

        close(clnt_sock);

    }

    close(serv_sock);


    return 0;

}


//에러 처리
void error_handle(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}