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
        char acao[1000], clean_buffer;

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
            printf("Entre com a requisicao (cadastrar/buscar): ");
            //scanf("%s", acao);
            //clean_buffer = getchar();
            strcpy(acao, "buscar");
            if(!strcmp(acao, "cadastrar"))
            {
                if(send(sock, acao, strlen(acao), 0) < 0)
                {
                    puts("Falha ao enviar requisicao\n");
                    return 1;
                }
                memset(acao, '\0', strlen(acao));
                if(recv(sock, server_reply, 2000, 0) < 0)
                {
                    puts("recv falhou\n");
                    break;
                }

                puts(server_reply);
                if(!strcmp(server_reply, "Entre com o nome do livro: "))
                {
                    memset(server_reply, '\0', strlen(server_reply));
                    fgets(book_name, 1000, stdin);
                    
                    if(send(sock, book_name, strlen(book_name), 0) < 0)
                    {
                        puts("Falha ao enviar requisicao\n");
                        return 1;
                    }
                    
                    if(recv(sock, server_reply, 2000, 0) < 0)
                    {
                        puts("recv falhou\n");
                        break;
                    }
                    memset(book_name, '\0', strlen(book_name));   
                    puts(server_reply);
                    memset(server_reply, '\0', strlen(server_reply));
                }
            }
            else if(!strcmp(acao, "buscar"))
            {
                if(send(sock, acao, strlen(acao), 0) < 0)
                {
                    puts("Falha ao enviar requisicao\n");
                    return 1;
                }
                memset(acao, '\0', strlen(acao));
                if(recv(sock, server_reply, 2000, 0) < 0)
                {
                    puts("recv falhou\n");
                    break;
                }

                puts(server_reply);
                if(!strcmp(server_reply, "Entre com o nome do livro a ser buscado: "))
                {
                    memset(server_reply, '\0', strlen(server_reply));
                    //fgets(book_name, 1000, stdin);
                    strcpy(book_name,"acer\n");
                    if(send(sock, book_name, strlen(book_name), 0) < 0)
                    {
                        puts("Falha ao enviar requisicao\n");
                        return 1;
                    }
                    
                    if(recv(sock, server_reply, 2000, 0) < 0)
                    {
                        puts("recv falhou\n");
                        break;
                    }
                    memset(book_name, '\0', strlen(book_name));   
                    puts(server_reply);
                    memset(server_reply, '\0', strlen(server_reply));
                }
            }
            else
            {
                printf("Comando nao identificado\n");
            }
        }
        close(sock);
        return 0;
    }