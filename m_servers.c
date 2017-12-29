#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t semaphore_server;
        

void *connection_handler(void *);

    int main()
    {
        int socket_desc, client_sock, c, *new_sock, nbytes;
        struct sockaddr_in server, client;
        int fd[2];
        char readbuffer[1000];
        sem_init(&semaphore_server, 0, 1);

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
        pipe(fd);
        puts("Aguardando requisicoes . . .\n");
        c = sizeof(struct sockaddr_in);
        while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
        {
            pthread_t sniffer_thread;
            new_sock = malloc(1);
            *new_sock = client_sock;   
            if(pthread_create(&sniffer_thread, NULL, connection_handler, (void*)new_sock) < 0)
            {
                perror("Nao foi possivel criar thread\n");
                return 1;
            }
            puts("Conexao estabelecida\n");
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
        int a;
        int found = 0;
        char *insercao_ok = "Novo livro registrado\n";
        char *requsicao_desconhecida = "Comando desconhecido\n";
        char *solicita_cadastro = "Entre com o nome do livro: ";
        char *solicita_busca = "Entre com o nome do livro a ser buscado: ";
        char *nenhum_cadastro = "Nenhum cadastro recebido\n";
        char *message, client_message[2000], save_buffer[2000];
        char *field;
        FILE *fp;
        
        while((read_size = recv(sock, client_message, 2000, 0)) > 0)
        {

            sem_wait(&semaphore_server);
            if(!strcmp(client_message, "cadastrar"))
            {
                memset(client_message, '\0', strlen(client_message));
                write(sock, solicita_cadastro, strlen(solicita_cadastro));
                if((read_size = recv(sock, client_message, 2000, 0)) > 0)
                {
                    client_message[strlen(client_message) - 1] = '\0';  
                    int size_register = strlen(client_message);
                    fp = fopen("memoria_compartilhada.txt", "a+");
                    fwrite(&size_register, sizeof(int), 1, fp);
                    fwrite(client_message, strlen(client_message), 1, fp);
                    fclose(fp);
                    memset(client_message, '\0', strlen(client_message));
                    write(sock, insercao_ok, strlen(insercao_ok));
                }

            }
            else if(!strcmp(client_message, "buscar"))
            {
                sem_getvalue(&semaphore_server, &a);
                printf("%d\n", a);
                memset(client_message, '\0', strlen(client_message));
                write(sock, solicita_busca, strlen(solicita_busca));
                if((read_size = recv(sock, client_message, 2000, 0)) > 0)
                {   
                    int tam;
                    char read_buffer[2000];
                    client_message[strlen(client_message) - 1] = '\0';     
                    fp = fopen("memoria_compartilhada.txt", "r");
                    rewind(fp);
                    found = 0;
                    while(fread(&tam, sizeof(int), 1, fp) && !found)
                    {
                        fread(read_buffer, tam, 1, fp);
                        strcpy(save_buffer, read_buffer);
                        field = strtok(save_buffer, "|");
                        if(!strcmp(client_message, field))
                        {
                            //printf("%s\n", read_buffer);
                            found = 1;
                            write(sock, read_buffer, strlen(read_buffer));
                        }
                        memset(read_buffer, '\0', strlen(read_buffer));
                        memset(save_buffer, '\0', strlen(save_buffer));
                    }
                    if(!found)
                    {
                        write(sock, nenhum_cadastro, strlen(nenhum_cadastro));
                    }
                    memset(client_message, '\0', strlen(client_message));
                    fclose(fp);

                }
                else
                {
                    write(sock, nenhum_cadastro, strlen(nenhum_cadastro));
                }
            }
            else
            {
                write(sock, requsicao_desconhecida, strlen(requsicao_desconhecida));
            }
            sem_post(&semaphore_server);

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