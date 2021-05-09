#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

//애러 처리
void error_handling(char* message);

//시그널 핸들러
void read_childproc(int sig);

int main(int argc, char* argv[])
{
    //소켓 디스크립터
    int serv_sock, clnt_sock;
    //소켓 주소 구조체
    struct sockaddr_in serv_adr, clnt_adr;

    //파이프 입출력 디스크립터
    int fds[2];

    //프로세스 id
    pid_t pid;

    //시그널 액션 정보
    struct sigaction act;

    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];



    if (argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    //sigaction 구조체 초기화
    act.sa_handler = read_childproc;          // 시그널 핸들러는 read_childproc 함수 포인터
    sigemptyset(&act.sa_mask);               //0으로 초기화
    act.sa_flags = 0;                        //0으로 초기화
    state = sigaction(SIGCHLD, &act, 0);    //시그널 등록 - 자식 프로세스 종료 관찰..

    //소켓 생성 - IPv4, TCP
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    //서버주소 정보 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    //서버 주소정보 할당
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind error - ");
    }

    //클라이언트 요청 준비 완료
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen error - ");
    }

    //파이프 생성
    pipe(fds);

    //자식 프로세스 생성 - 파이프 파일 디스크립터 복사됨.
    pid = fork();

    //자식
    if (pid == 0) {

        printf("i am child [1] \n");

        //쓰기 전용으로 파일열기
        FILE* fp = fopen("echomsg3.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for (i = 0; i < 10; i++) {
            printf("for문 들어옴 -  \n");
            //부모가 데이터 보낼때까지 read 함수에서 블로킹
            //파이프 입력 버퍼로 부모 프로세스가 보낸 데이터 수신
            len = read(fds[0], msgbuf, BUF_SIZE);     //부모 프로세스로부터 데이터를 수신하여

            printf("파일에 쓸 길이 len : %d \n", len);
            printf("파일에 쓸 내용 : %s \n", msgbuf);

            fwrite(msgbuf, 1, len, fp);        //파일에 씀
        }

        fclose(fp); //파일 닫고
        return 0;     //10번 이상 호출 된 후 자식 프로세스 종료

    }


    //부모 프로세스 - 클라이언트 요청 처리한다.

    while (1) {

        //서버가 클라이언트 요청 수락
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

        if (clnt_sock == -1) {
            continue;
        }
        else {
            puts("new client connected...\n");
        }


        pid = fork();           //서버가 자식 프로세스 생성

//            if (pid == -1) {        //자식 프로세스 생성 실패 했다면
//                close(clnt_sock);   //클라이언트와 연결을 위해 생성한 소켓 연결종료
//                continue;
//            }

            //자식 프로세스면
        if (pid == 0) {

            printf("i am child [2] \n");

            close(serv_sock); //서버의 부모 프로세스만 이소켓에 대한 파일 디스크립터 가짐

            //클라이언트로 부터 데이터 모두 수신해서
            while (1) {
                printf("while 문 진입 - read () 전.. \n");

                str_len = read(clnt_sock, buf, BUF_SIZE);

                printf("str_len : %d \n", str_len);

                if (str_len == 0) {
                    break;
                }

                //다시 클라이언트로 에코 - 1
                write(clnt_sock, buf, str_len);

                //파이프 출력 버퍼로 데이터 출력하기 - 2
                write(fds[1], buf, str_len);
            }


            //클라이언트에 대한 에코 서비스를 모두 완료했으므로 소켓 연결 종료
            close(clnt_sock);
            puts("client disconnected...");

            return 0;   //클라이언트가 q를 입력하면 자식 프로세스 종료


        }
        else {
            //서버의 부모 프로세스의 경우
            //서버의 자식 프로세스만 클라이언트 소켓에 대한 파일 디스크립터를 갖게 하기 위함
            close(clnt_sock);
        }


    }//while - end



//서버의 부모 프로세스에서 소켓 종료
    close(serv_sock);
    return 0;
}


//시그널 핸들러
void read_childproc(int sig) {
    pid_t pid;
    int status;
    //임의의 자식 프로세스가 종료하길 대기
    pid = waitpid(-1, &status, WNOHANG);

    if (WIFEXITED(status)) {
        printf("removed proc id : %d \n", pid);
    }


}


//에러처리
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}