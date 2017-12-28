all: client m_servers
client: client.c 
	gcc -o client client.c
m_servers: m_servers.c 
	gcc -o m_servers m_servers.c -lpthread
