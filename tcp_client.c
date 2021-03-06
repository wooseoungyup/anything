#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);


int main(int argc, char* argv[]) {

    //소켓 파일 디스크립터
    int sock;
    struct sockaddr_in serv_addr;

    //서버로 부터 받을 메시지 사이즈
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    //인자 개수 체크
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }


    //소켓 생성 - ipv4 , tcp
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    //주소정보 셋팅할 구조체 0으로 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //연결
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        error_handling("connect() error");
    }

    //서버에서 받은 메시지 읽기
    
    while (read_len = read(sock, &message[idx++], 1))
    {
        if (read_len == -1)
            error_handling("read() error");

        str_len += read_len;
    }


    printf("message from server : %s \n", message);
    printf("function read call count : %d \n", str_len);

    //소켓 닫기
    close(sock);


    return 0;
}




//에러처리
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}