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

//�ַ� ó��
void error_handling(char* message);

//�ñ׳� �ڵ鷯
void read_childproc(int sig);

int main(int argc, char* argv[])
{
    //���� ��ũ����
    int serv_sock, clnt_sock;
    //���� �ּ� ����ü
    struct sockaddr_in serv_adr, clnt_adr;

    //������ ����� ��ũ����
    int fds[2];

    //���μ��� id
    pid_t pid;

    //�ñ׳� �׼� ����
    struct sigaction act;

    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];



    if (argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    //sigaction ����ü �ʱ�ȭ
    act.sa_handler = read_childproc;          // �ñ׳� �ڵ鷯�� read_childproc �Լ� ������
    sigemptyset(&act.sa_mask);               //0���� �ʱ�ȭ
    act.sa_flags = 0;                        //0���� �ʱ�ȭ
    state = sigaction(SIGCHLD, &act, 0);    //�ñ׳� ��� - �ڽ� ���μ��� ���� ����..

    //���� ���� - IPv4, TCP
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    //�����ּ� ���� �ʱ�ȭ
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    //���� �ּ����� �Ҵ�
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind error - ");
    }

    //Ŭ���̾�Ʈ ��û �غ� �Ϸ�
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen error - ");
    }

    //������ ����
    pipe(fds);

    //�ڽ� ���μ��� ���� - ������ ���� ��ũ���� �����.
    pid = fork();

    //�ڽ�
    if (pid == 0) {

        printf("i am child [1] \n");

        //���� �������� ���Ͽ���
        FILE* fp = fopen("echomsg3.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;

        for (i = 0; i < 10; i++) {
            printf("for�� ���� -  \n");
            //�θ� ������ ���������� read �Լ����� ���ŷ
            //������ �Է� ���۷� �θ� ���μ����� ���� ������ ����
            len = read(fds[0], msgbuf, BUF_SIZE);     //�θ� ���μ����κ��� �����͸� �����Ͽ�

            printf("���Ͽ� �� ���� len : %d \n", len);
            printf("���Ͽ� �� ���� : %s \n", msgbuf);

            fwrite(msgbuf, 1, len, fp);        //���Ͽ� ��
        }

        fclose(fp); //���� �ݰ�
        return 0;     //10�� �̻� ȣ�� �� �� �ڽ� ���μ��� ����

    }


    //�θ� ���μ��� - Ŭ���̾�Ʈ ��û ó���Ѵ�.

    while (1) {

        //������ Ŭ���̾�Ʈ ��û ����
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

        if (clnt_sock == -1) {
            continue;
        }
        else {
            puts("new client connected...\n");
        }


        pid = fork();           //������ �ڽ� ���μ��� ����

//            if (pid == -1) {        //�ڽ� ���μ��� ���� ���� �ߴٸ�
//                close(clnt_sock);   //Ŭ���̾�Ʈ�� ������ ���� ������ ���� ��������
//                continue;
//            }

            //�ڽ� ���μ�����
        if (pid == 0) {

            printf("i am child [2] \n");

            close(serv_sock); //������ �θ� ���μ����� �̼��Ͽ� ���� ���� ��ũ���� ����

            //Ŭ���̾�Ʈ�� ���� ������ ��� �����ؼ�
            while (1) {
                printf("while �� ���� - read () ��.. \n");

                str_len = read(clnt_sock, buf, BUF_SIZE);

                printf("str_len : %d \n", str_len);

                if (str_len == 0) {
                    break;
                }

                //�ٽ� Ŭ���̾�Ʈ�� ���� - 1
                write(clnt_sock, buf, str_len);

                //������ ��� ���۷� ������ ����ϱ� - 2
                write(fds[1], buf, str_len);
            }


            //Ŭ���̾�Ʈ�� ���� ���� ���񽺸� ��� �Ϸ������Ƿ� ���� ���� ����
            close(clnt_sock);
            puts("client disconnected...");

            return 0;   //Ŭ���̾�Ʈ�� q�� �Է��ϸ� �ڽ� ���μ��� ����


        }
        else {
            //������ �θ� ���μ����� ���
            //������ �ڽ� ���μ����� Ŭ���̾�Ʈ ���Ͽ� ���� ���� ��ũ���͸� ���� �ϱ� ����
            close(clnt_sock);
        }


    }//while - end



//������ �θ� ���μ������� ���� ����
    close(serv_sock);
    return 0;
}


//�ñ׳� �ڵ鷯
void read_childproc(int sig) {
    pid_t pid;
    int status;
    //������ �ڽ� ���μ����� �����ϱ� ���
    pid = waitpid(-1, &status, WNOHANG);

    if (WIFEXITED(status)) {
        printf("removed proc id : %d \n", pid);
    }


}


//����ó��
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}