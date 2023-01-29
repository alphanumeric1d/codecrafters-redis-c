#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


void* respond(void* sock){

	char buf[256];
	int reading;

	printf("CCCCC");

	while(reading = read(*(int*)sock, buf, sizeof(buf)-1) < 0);

	printf("DDDDD");

	printf("%d", reading);
	const char* response2ping = "+PONG\r\n";

	printf("EEEEE");
	int send_response = write(*(int*)sock, response2ping, sizeof(response2ping)-1);

	printf("%d", send_response);

	if (send_response < 0) {
		printf("sendind response failed");
	}

	//return NULL;
}


int main() {
        // Disable output buffering
        setbuf(stdout, NULL);

        // You can use print statements as follows for debugging, they'll be visible when running tests.

	int server_fd, client_addr_len;
        struct sockaddr_in client_addr;

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
             printf("Socket creation failed: %s...\n", strerror(errno));
             return 1;
        }

        // Since the tester restarts your program quite often, setting REUSE_PORT
        // ensures that we don't run into 'Address already in use' errors
        int reuse = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
             printf("SO_REUSEPORT failed: %s \n", strerror(errno));
             return 1;
        }

        struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
	.sin_port = htons(6379),
	.sin_addr = { htonl(INADDR_ANY) },
	};

        if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
             printf("Bind failed: %s \n", strerror(errno));
             return 1;
        }

        int connection_backlog = 5;
        if (listen(server_fd, connection_backlog) != 0) {
             printf("Listen failed: %s \n", strerror(errno));
             return 1;
        }

        printf("Waiting for a client to connect...\n");
        client_addr_len = sizeof(client_addr);
	printf("Client connected\n");

	pthread_t thread[10];
	size_t id = 0;
	int sockid = 0;
	int socket[10];

	while(1) {
		while (socket[sockid] = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len));
			if (socket[sockid] < 0) {
				printf("accept failed");
			}
			printf("socket: %d", socket[sockid]);

			printf("AAAAA");

			if(pthread_create(&thread[id++], NULL, &respond, &socket[sockid++]) != 0){

				printf("failed");

			}
			printf("BBBBB");
		}

		for (int i = 0; i < 10; i++) {
			if(pthread_join(thread[i], NULL) != 0){
				printf("failed returning");
			};
	}

        close(server_fd);

        return 0;
}
