// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 2000
#define CONNECTS 10
#define NAME "CosmoDog"

char* connection_accept = "Connection accepted. Hello, my name is "NAME".";
char* connection_deny_overload = "Connection denied. I'm not support more than 10 connections";
char* connection_deny_clone = "Connection denied. I have connection with your IP already";

int memset(int*, int, size_t);

fd_set connect_set;
int connect_sockets[CONNECTS];
struct sockaddr_in clients_info[CONNECTS];

void* linker(void* data) {
	printf("I'm here\n");
	int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in server_info;
	server_info.sin_family = AF_INET;
	server_info.sin_port = SERVER_PORT;
	server_info.sin_addr.s_addr = INADDR_ANY;

	bind(listen_socket, (struct sockaddr*)&server_info, sizeof(struct sockaddr));

	listen(listen_socket, 10);

	fd_set readfd;
	FD_ZERO(&readfd);
	FD_SET(listen_socket, &readfd);

	while (1) {
		int n_socket;
		struct sockaddr_in client_info;
		printf("Now here\n");
		n_socket = accept(listen_socket, (struct sockaddr*)&client_info, NULL);
		printf("Or here\n");
		fflush(stdout);
		int i;
		for (i = 0; i < CONNECTS; ++i)
			if (!connect_sockets[i])
				break;
		if (i != CONNECTS) {
			int j;
			for (j = 0; j < CONNECTS; ++j)
				if (!strcmp(client_info.sin_addr, clients_info[j].sin_addr))
					break;
			if (j != CONNECTS) {
				printf("Hehe\n");
				connect_sockets[i] = n_socket;
				clients_info[i] = client_info;
				FD_SET(n_socket, &connect_set);
				send(n_socket, connection_accept, sizeof(connection_accept), 0);
			}
			else {
				send(n_socket, connection_deny_clone, sizeof(connection_deny_clone), 0);
				close(n_socket);
			}
		}
		else {
			send(n_socket, connection_deny_overload, sizeof(connection_deny_overload), 0);
			close(n_socket);
		}
		printf("gat\n");
	}
}

void* server(void* data) {

}

int main()
{
	memset(connect_sockets, 0, CONNECTS);
	
	pthread_t linker_thread;

	pthread_create(&linker_thread, NULL, linker, NULL);
	pthread_join(linker_thread, NULL);
}