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

    //���� ���� üũ
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    //���� ����
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        error_handle("socket() error");
    }


    //�ּ� ���� , ip, port tptxld
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));


    //����!
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handle("connect() error");
    }
    else {

        printf("connected .....");
    }



    while (1) {
        //Ŀ�ǵ� ���ο� ǥ�� ���
        fputs("Input message(Q to quit) : ", stdout);

        //Ŀ�����ο� �Է��Ѱ� ǥ�� �Է¹���
        fgets(message, BUF_SIZE, stdin);
        //�� ���ڿ��� ������ 0
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }

        //�������Ͽ� ����
        write(sock, message, strlen(message));

        //������������ ���� ���� ���� �б�
        str_len = read(sock, message, BUF_SIZE - 1);

        //�޽��� ���� 0 ���� �ʱ�ȭ
        message[str_len] = 0;

        //���� ���� ���
        printf("message from server : %s", message);
    }

    //������ ���� �޽��� ������
    close(sock);



    return 0;

}


//���� ó��
void error_handle(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}