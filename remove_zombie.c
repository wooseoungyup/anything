#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>


void read_childproc(int sig) {

    int status;

    /*
     ������ �ڽ� ���μ��� ���Ḧ ��ٸ�
     ���� ����� �ڽ� ���μ����� ���ٸ� 0�� ��ȯ
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

    //sigaction ����ü �ʱ�ȭ
    act.sa_handler = read_childproc;    //�ñ׳� �ڵ鷯�� read_childproc �Լ� ������
    sigemptyset(&act.sa_mask);          //0���� �ʱ�ȭ
    act.sa_flags = 0;                   //0���� �ʱ�ȭ

    //SIGCHLD : �ڽ� ���μ����� ����Ǹ� �˷��޶�� Ŀ�ο� ���
    sigaction(SIGCHLD, &act, 0);

    //�ڽ� ���μ��� ����
    pid = fork();
    //�ڽ� ���μ����� ���
    if (pid == 0) {
        puts("hi i am child process[1]");
        sleep(10);  //10�� ���ŷ
        return 12;   //12 ����
    }
    else {
        //�θ� ���μ����� ���
        printf("haha i am parent process[1] !\n");
        printf("my child process[1] id : %d \n", pid);
        //���ο� �ڽ� ���μ��� ����
        pid = fork();
        if (pid == 0) {
            puts("hi i am child process[2]");
            sleep(10);  //10�� ���ŷ
            exit(24);   //24 �����ϸ� ����
        }
        else {

            //�θ� ���μ��� �ϰ��
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