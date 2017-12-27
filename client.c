#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

    int main()
    {
        int sock;
        struct sockaddr_in server;
        char book_name[1000], server_reply[2000];

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            printf("Nao foi possivel criar socket\n");
        }
        puts("Socket criado");

        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons(8888);

        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("Conexao falhou, erro\n");
            return 1;
        }
        puts("Conectado\n");

        while(1)
        {
            printf("Entre com o nome do livro: ");
            scanf("%s", book_name);
            
            if(send(sock, book_name, strlen(book_name), 0) < 0)
            {
                puts("Falha ao enviar requisiçao\n");
                return 1;
            }
            
            if(recv(sock, server_reply, 2000, 0) < 0)
            {
                puts("recv falhou\n");
                break;
            }

            puts("Resposta do servidor: ");
            puts(server_reply);
        }
        close(sock);
        return 0;
    }