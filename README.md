Trabalho de Sistemas Operacionais
Prof. Flávio Braga

Cliente servidor
Cadastro e buscas de livros

Alunos:
Anderson Dario ra 90093
Carlos Kanda ra 94090
Thiago Kira ra 78750

Utilização:
$ make

$ ./m_servers

(Em outro terminal)

$ ./client

Usar duas funcoes: cadastrar e buscar

Cliente servidor (linux) em C
Thread e socket para comunicação entre os processos: varios clientes - servidor
Arquivo como memoria compartilhada
Semaforo para gerenciar requisições de escrita/leitura da memoria compartilhada
fork/fifo: processo pai passa um cadastro para o processo filho e este cadastra novo registro na memoria compartilhada
pipe para passar mensagem de status entre os processos
Duas interfaces: cadastrar e buscar