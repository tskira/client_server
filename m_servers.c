#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *connection_handler(void *);

    int main()
    {
        int socket_desc, client_sock, c, *new_sock;
        struct sockaddr_in server, client;

        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_desc == -1)
        {
            printf("Nao foi possivel criar socket\n");
        }
        puts("Socket criado\n");

        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(8888);

        if( bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            perror("Bind falhou\n");
            return 1;
        }
        puts("Bind feito\n");

        listen(socket_desc, 3);

        puts("Aguardando requisicoes . . .\n");
        c = sizeof(struct sockaddr_in);
        while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
        {
            puts("Conexao aceita\n");

            pthread_t sniffer_thread;
            new_sock = malloc(1);
            *new_sock = client_sock;

            if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*)new_sock) < 0)
            {
                perror("Nao foi possivel criar thread\n");
                return 1;
            }
            puts("Associado\n");
        }

        if(client_sock < 0)
        {
            perror("Falhou requisicao\n");
            return 1;
        }

        return 0;
    }

    void *connection_handler(void *socket_desc)
    {
        int sock = *(int*)socket_desc;
        int read_size;
        char *insercao_ok = "Novo livro registrado\n";
        char *message, client_message[2000];
        FILE *fp;
        /*
        message = "Parabens, eu sou sua conexao\n";
        write(sock, message, strlen(message));

        message = "Agora escreva alguma coisa ai \n";
        write(sock, message, strlen(message));
        */

        while((read_size = recv(sock, client_message, 2000, 0)) > 0)
        {
            int size_register = strlen(client_message);
            fp = fopen("memoria_compartilhada.txt", "a+");
            fwrite(&size_register, sizeof(int), 1, fp);
            fwrite(client_message, size_register, 1, fp);
            fclose(fp);
            write(sock, insercao_ok, strlen(insercao_ok));
        }

        if(read_size == 0)
        {
            puts("Cliente desconectado\n");
            fflush(stdout);
        }
        else if(read_size == -1)
        {
            perror("recv falhou\n");
        }

        free(socket_desc);

        return 0;
    }