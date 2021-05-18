#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>         // O_WRONLY
#include <unistd.h>        // write(), close()

#define TTL 64
#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[])
{

    int send_sock;
    struct sockaddr_in mul_adr;
    int time_live = TTL;
    //FILE *fp;
    int fd;
    char buf[BUF_SIZE];

    //��� /��Ƽĳ��Ʈ IP / PORT��ȣ
    if (argc != 3) {
        printf("usage : %s <groupIP> <PORT> \n", argv[0]);
        exit(1);
    }

    //UDP ���� ����
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);


    //��Ƽ ĳ��Ʈ �ּ� ���� �ʱ�ȭ
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);    //��Ƽĳ��Ʈ ip
    mul_adr.sin_port = htons(atoi(argv[2]));         //��Ƽĳ��Ʈ ��Ʈ

    //TTL ����
    setsockopt(send_sock, IPPROTO_IP,
        IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

    //������ �б� �������� ����
//    fp = fopen("news.txt", "r");
//    if (fp == NULL) {
//        error_handling("fopen error");
//    }
//

    fd = open("news.txt", O_RDONLY);
    if (fd == -1) {
        error_handling("open error");
    }



    //broadcasting
    while (1) {

        //���Ͽ��� �����͸� 30 ����Ʈ �о
        //char *result = fgets(buf, BUF_SIZE , fp);

        ssize_t result = read(fd, buf, BUF_SIZE);

        if (result == 0) {
            break;
        }

        //printf("���� ������ buf : %s \n" , buf);

        //��Ƽĳ��Ʈ �ּҷ� ������ ����
        sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));

        //������ ���ۿ� �ð��� �ѷ���.
        sleep(1);
    }



    //fclose(fp);         //���� �ݱ�
    close(fd);       //low ���� ���� �ݱ�
    close(send_sock);   //���� ���� ����


    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}