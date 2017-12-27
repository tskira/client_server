#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


    int main()
    {
        int socket_desc, client_sock, c, read_size;
        struct sockaddr_in server, client;
        char client_message[2000];
        char *final_message = "Vao se foderem";

        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1)
        {
            printf("Não foi possível criar socket\n");
        }
        puts("Socket created");

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(8888);

        if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("Bind falhou\n");
            return 1;
        }
        puts("Bind feito\n");

        listen(socket_desc, 3);

        puts("Aguardando requisicoes...\n");
        c = sizeof(struct sockaddr_in);

        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("Falha");
            return 1;
        }
        puts("Conexao aceita\n");
        
        while((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
        {
            //write(client_sock, client_message, strlen(client_message));
            write(client_sock, final_message, strlen(final_message));
        }

        if (read_size == 0)
        {
            puts("Cliente desconectado\n");
            fflush(stdout);
        }
        else if (read_size == -1)
        {
            perror("recv falhou");
        }

        return 0;
    }