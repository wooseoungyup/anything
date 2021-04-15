#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(char* message);

int main(int argc, char* argv[]) {

    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    //인자 개수 체크
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        error_handle("socket() error");
    }


    //주소 설정 , ip, port tptxld
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));


    //연결!
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handle("connect() error");
    }
    else {

        printf("connected .....");
    }



    while (1) {
        //커맨드 라인에 표준 출력
        fputs("Input message(Q to quit) : ", stdout);

        //커멘드라인에 입력한것 표준 입력받음
        fgets(message, BUF_SIZE, stdin);
        //두 문자열이 같으면 0
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        //서버소켓에 쓰기
        write(sock, message, strlen(message));

        //서버소켓으로 부터 받은 문자 읽기
        str_len = read(sock, message, BUF_SIZE - 1);

        //메시지 버퍼 0 으로 초기화
        message[str_len] = 0;

        //받은 문자 출력
        printf("message from server : %s", message);
    }

    //서버에 종료 메시지 날리기
    close(sock);



    return 0;

}


//에러 처리
void error_handle(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}