#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handle(char* message);

int main(int argc, char* argv[]) {

    //����, Ŭ���̾�Ʈ ���� ��ũ����
    int serv_sock, clnt_sock;
    //�޽��� ����
    char message[BUF_SIZE];

    int str_len, i;

    //�ּ� ���� ����ü
    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    //�͹̳� ���ڰ��� üũ
    if (argc != 2) {
        printf("usage : %s <port> \n", argv[0]);
        exit(1);
    }

    //���� ���� : ipv4 ,tcp
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handle("socket() error");
    }
    else {
        printf("socket success ! \n");
    }

    //�ּ� ������ ���� �޸� 0���� �ʱ�ȭ, ip, port ����
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    //���ε� - �ּ� ����
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        error_handle("bind error");
    }
    else {
        printf("bind success ! \n");
    }

    //��� ť 5�� ����
    if (listen(serv_sock, 5) == -1) {
        error_handle("listen error");
    }
    else {
        printf("listen success ! \n");
    }

    //Ŭ���̾�Ʈ �ּҼ��� ����
    clnt_adr_sz = sizeof(clnt_adr);

    //Ŭ���̾�Ʈ�� 5�� �Ǵ� 5���� Ŭ���̾�Ʈ�� ���� �����ϴ�.
    for (i = 0; i < 5; i++) {

        printf("Entering for-loop i : %d \n", i);

        //���� - Ŭ���̾�Ʈ ���� �����....
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        if (clnt_sock == -1) {
            error_handle("accept error");
        }
        else {
            printf("accept success \nconnected client %d \n", i + 1);
        }


        //�������� Ŭ���̾�Ʈ�� ���� �޽��� �о �ٽ� Ŭ���̾�Ʈ�� ���� ó��
        while (1) {

            //#.����
            //�ϳ��� Ŭ���̾�Ʈ�� ���͸� ���ÿ� �д� ����... (���� Ŭ���̾�Ʈ�� ó���� �� ����..)
            //while ���� ��� �ɵ���,
            //Ŭ���̾�Ʈ�� q�� ������ ���ᰡ �Ǿ��.. �״��� Ŭ���̾�Ʈ ������ ó���ȴ�....
            str_len = read(clnt_sock, message, BUF_SIZE);

            //Ŭ���̾�Ʈ���� ���� ���� ����Ʈ ��
            printf("str_len : %d \n", str_len);

            if (str_len != 0) {
                write(clnt_sock, message, str_len);
            }
            else {
                printf("break \n");
                break;
            }

        }

        close(clnt_sock);

    }

    close(serv_sock);


    return 0;

}


//���� ó��
void error_handle(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}