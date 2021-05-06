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

//����ó��
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//�ñ׳� �ڵ鷯
void read_childproc(int sig) {
    pid_t pid;
    int status;
    //������ �ڽ� ���μ����� ����Ǳ� ��ٸ�
    pid = waitpid(-1, &status, WNOHANG);

    //�ڽ� ���μ����� ���� ����ɶ� true ��ȯ
    if (WIFEXITED(status)) {
        printf("removed proc id : %d \n", pid);                    //����� �ڽ����μ���
        printf("removed proc send : %d \n", WEXITSTATUS(status));  //���޵� ��
    }
}

int main(int argc, char* argv[])
{
    //����, Ŭ���̾�Ʈ ����
    int serv_sock, clnt_sock;
    //���Ͽ� �Ҵ��� �ּ� ���� ����ü
    struct sockaddr_in serv_adr, clnt_adr;

    //���μ��� id
    pid_t pid;
    //�ñ׳� ���� ����ü
    struct sigaction act;

    //Ŭ���̾�Ʈ ���� ����
    socklen_t adr_sz;

    int str_len, state;
    //������ �����͸� ���� char �迭
    char buf[BUF_SIZE];


    //���������� ��� + port �� ���ڷ� �޴´�.
    if (argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    //sigaction ����ü �ʱ�ȭ
    act.sa_handler = read_childproc;    //�ñ׳� �ڵ鷯�� read_childproc �Լ�������
    sigemptyset(&act.sa_mask);          //0���� �ʱ�ȭ
    act.sa_flags = 0;                  //0���� �ʱ�ȭ

    //�ڽ� ���μ��� ����� read_childproc �Լ� ȣ��ǰ� �ü������ ���
    state = sigaction(SIGCHLD, &act, 0);  //�ü���� �ñ׳� ���

    //IPv4, TCP ���� ����
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);


    //�����ּ� ���� �ʱ�ȭ
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));


    //���� �ּ������� ������� �ּ� �Ҵ�
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handling("bind() error...");
    }


    //������ Ŭ���̾�Ʈ�� ���� ��û �غ� �Ϸ�
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

            //�ڽ� ���μ��� ����
            pid = fork();

            //���μ��� ���� ���� ó��
            if (pid == -1) {
                continue;
            }

            //�ڽ����μ��� �ΰ��
            if (pid == 0) {

                close(serv_sock);   //�ڽ� ���μ����� �������� ���� ��ũ���� �ʿ����.

                while (1) {

                    //Ŭ���̾�Ʈ ���� ���� ������ �о
                    str_len = read(clnt_sock, buf, BUF_SIZE);

                    if (str_len == 0) {
                        printf("str_len == 0 \n");
                        break;
                    }
                    //Ŭ���̾�Ʈ�� �ٽ� ����..(����..)
                    write(clnt_sock, buf, str_len);

                }

                //Ŭ���̾�Ʈ�� ���� ���� ���񽺸� ��� �Ϸ� �����Ƿ� ���� ���� ����
                close(clnt_sock);
                puts("client disconnected...");
                //�ڽ� ���μ��� ����
                return 0;

            }
            else {
                //�θ� ���μ��� ���
                //Ŭ���̾�Ʈ ������ �ʿ����. - �ڽĸ� �ʿ�
                close(clnt_sock);

            }


        }

    }

    //�θ� ���μ������� ���� ���� ����
    close(serv_sock);
    return 0;
}
