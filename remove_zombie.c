#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


void read_childproc(int sig) {

    int status;

    /*
     임의의 자식 프로세스 종료를 기다림
     현재 종료된 자식 프로세스가 없다면 0을 반환
     */
    pid_t pid = waitpid(-1, &status, WNOHANG);


    if (WIFEXITED(status)) {

        printf("removed proc id : %d \n", pid);

        printf("child send: %d \n", WEXITSTATUS(status));

    }

}


int main(int argc, char* argv[])
{
    pid_t pid;
    struct sigaction act;

    //sigaction 구조체 초기화
    act.sa_handler = read_childproc;    //시그널 핸들러는 read_childproc 함수 포인터
    sigemptyset(&act.sa_mask);          //0으로 초기화
    act.sa_flags = 0;                   //0으로 초기화

    //SIGCHLD : 자식 프로세스가 종료되면 알려달라고 커널에 등록
    sigaction(SIGCHLD, &act, 0);

    //자식 프로세스 생성
    pid = fork();
    //자식 프로세스일 경우
    if (pid == 0) {
        puts("hi i am child process[1]");
        sleep(10);  //10초 블로킹
        return 12;   //12 리턴
    }
    else {
        //부모 프로세스일 경우
        printf("haha i am parent process[1] !\n");
        printf("my child process[1] id : %d \n", pid);
        //새로운 자식 프로세스 생성
        pid = fork();
        if (pid == 0) {
            puts("hi i am child process[2]");
            sleep(10);  //10초 블로킹
            exit(24);   //24 전달하며 종료
        }
        else {

            //부모 프로세스 일경우
            int i;
            printf("haha i am parent process[2] !\n");
            printf("my child process[2] id : %d \n", pid);

            for (i = 0; i < 5; i++) {
                puts("wait.....");
                sleep(25);
            }

        }

    }
    return 0;
}