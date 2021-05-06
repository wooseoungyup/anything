#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>

#define BUF_SIZE 1024
void error_handling(char* message);

//�б� ó�� (�Է�)
void read_routine(int sock, char* buf);

//���� ó�� (���)
void write_routine(int sock, char* buf);

int main(int argc, char* argv[])
{

    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;


    //������ + ip + port ���ڷ� �޴´�.
    if (argc != 3) {
        printf("usage :  %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    //TCP ���� ����
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        error_handling("socket() error");
    }

    //���� �ּ� ���� �ʱ�ȭ
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);  //ip
    serv_adr.sin_port = htons(atoi(argv[2]));       //port



    //���� �ּ������� ������� ���� ��û
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    {
        error_handling("connect() error");
    }
    else {

        puts("connected.....");

        pid = fork();   //�ڽ� ���μ��� ����
        if (pid == 0) {
            write_routine(sock, buf); //�ڽ� ���μ��� - ��� ��ƾó��
        }
        else {
            read_routine(sock, buf);  //�θ� ���μ��� - �Է� ��ƾó��
        }



    }

    close(sock);
    return 0;
}

//�Է¹ޱ� �Լ�
void read_routine(int sock, char* buf) {

    while (true)
    {

        int str_len = read(sock, buf, BUF_SIZE - 1); //������ ���� ������ ����
        //printf("read_routine - str_len : %d \n" , str_len);

        //�������� ������ ������ ����
        if (str_len == 0) {
            printf("str_len == 0 \n");
            return;
        }

        buf[str_len] = '\0';
        printf("message from server : %s \n", buf);

    }
}

//����ϱ� �Լ�
void write_routine(int sock, char* buf) {

    while (1)
    {
        fputs("Input message(Q to quit) : ", stdout);
        fgets(buf, BUF_SIZE, stdin); //ǥ���Է����� ���� �Է� ����

        //���� Ű�� ������
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            shutdown(sock, SHUT_WR); //��� ��Ʈ�� ����
            return;
        }

        //�ι��ڸ� ������ ���ڿ��� ������ �۽�
        write(sock, buf, strlen(buf));
        // printf("write - ������ ��� �Ϸ�. \n" );
    }
}


//���� ó��
void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}