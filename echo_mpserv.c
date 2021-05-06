#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

//에러처리
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//시그널 핸들러
void read_childproc(int sig) {
    pid_t pid;
    int status;
    //임의의 자식 프로세스가 종료되길 기다림
    pid = waitpid(-1, &status, WNOHANG);

    //자식 프로세스가 정상 종료될때 true 반환
    if (WIFEXITED(status)) {
        printf("removed proc id : %d \n", pid);                    //종료된 자식프로세스
        printf("removed proc send : %d \n", WEXITSTATUS(status));  //전달된 값
    }
}

int main(int argc, char* argv[])
{
    //서버, 클라이언트 소켓
    int serv_sock, clnt_sock;
    //소켓에 할당할 주소 정보 구조체
    struct sockaddr_in serv_adr, clnt_adr;

    //프로세스 id
    pid_t pid;
    //시그널 정보 구조체
    struct sigaction act;

    //클라이언트 소켓 길이
    socklen_t adr_sz;

    int str_len, state;
    //전달할 데이터를 담을 char 배열
    char buf[BUF_SIZE];


    //실행파일의 경로 + port 를 인자로 받는다.
    if (argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    //sigaction 구조체 초기화
    act.sa_handler = read_childproc;    //시그널 핸들러는 read_childproc 함수포인터
    sigemptyset(&act.sa_mask);          //0으로 초기화
    act.sa_flags = 0;                  //0으로 초기화

    //자식 프로세스 종료시 read_childproc 함수 호출되게 운영체제에게 등록
    state = sigaction(SIGCHLD, &act, 0);  //운영체제에 시그널 등록

    //IPv4, TCP 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);


    //서버주소 정보 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));


    //서버 주소정보를 기반으로 주소 할당
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error...");
    }


    //서버가 클라이언트의 연결 요청 준비를 완료
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error...");
    }


    while (true) {

        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

        if (clnt_sock == -1) {
            continue;
        }
        else {

            puts("new client connected...");

            //자식 프로세스 생성
            pid = fork();

            //프로세서 생성 오류 처리
            if (pid == -1) {
                continue;
            }

            //자식프로세스 인경우
            if (pid == 0) {

                close(serv_sock);   //자식 프로세스는 서버소켓 파일 디스크립터 필요없음.

                while (1) {

                    //클라이언트 에서 보낸 데이터 읽어서
                    str_len = read(clnt_sock, buf, BUF_SIZE);

                    if (str_len == 0) {
                        printf("str_len == 0 \n");
                        break;
                    }
                    //클라이언트에 다시 에코..(전달..)
                    write(clnt_sock, buf, str_len);

                }

                //클라이언트에 대한 에코 서비스를 모두 완료 했으므로 소켓 연결 종료
                close(clnt_sock);
                puts("client disconnected...");
                //자식 프로세스 종료
                return 0;

            }
            else {
                //부모 프로세스 경우
                //클라이언트 소켓은 필요없다. - 자식만 필요
                close(clnt_sock);

            }


        }

    }

    //부모 프로세스에서 서버 소켓 종료
    close(serv_sock);
    return 0;
}
