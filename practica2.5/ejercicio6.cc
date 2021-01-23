#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {

	if (argc < 2) {
		printf("Mal formato de comando, escribir: %s port\n",argv[0]);
		return -1;
	}

	struct addrinfo info;
	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = 0;
	info.ai_flags = AI_PASSIVE;
	info.ai_addr = NULL;
	info.ai_canonname = NULL;
	info.ai_next = NULL;

	struct addrinfo *list;
	int result = getaddrinfo("::", argv[1], &info, &list);

	if (result != 0) {
		printf("Error al hacer gettinf: %s\n", gai_strerror(result));
		return -1;
	}

	int on = 1;
	int off = 0;

	int sd = socket(list->ai_family, SOCK_STREAM, list->ai_protocol);
	if (sd == -1) {
		perror("Incapaz de abrir el socket\n");
		return -1;
	}

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int));
	setsockopt(sd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(int));

	if (bind(sd, list->ai_addr, list->ai_addrlen)) {
		perror("Incapaz de hacer bind\n");
		close(sd);
		return -1;
	}

	freeaddrinfo(list);

	char buf[257] = "";
	struct sockaddr_storage input_addr;
	socklen_t input_len = sizeof(input_addr);

	if (-1 == listen(sd, 10)) {
		perror("Wrror mientras se escuchaba\n");
		return -1;
	}

	while (1) {

		int asock = accept(sd, (struct sockaddr*)&input_addr, &input_len);
		if (asock == -1) {
			perror("Error mientras se aceptaba conexión\n");
			return -1;
		}

		char hostname[NI_MAXHOST];
		char port[NI_MAXSERV];
		if (getnameinfo((struct sockaddr*)&input_addr, input_len, hostname, NI_MAXHOST, port, NI_MAXSERV, 0)) {
			perror("Error al conseguir la información desde name");
			return -1;
		}
		printf("Conexión desde %s : %s\n", hostname, port);

		ssize_t readsize;
		do {
			readsize = recv(asock, buf, 256*sizeof(char), 0);
					
			if (readsize == -1) {
				perror("Error mientras se recibía\n");
				close(sd);
				return -1;
			}
			else if (readsize == 0) {
				printf("El cliente se ha desconectado\n");
			}
			else {
				buf[readsize] = '\0';
				ssize_t writesize = send(asock, buf, (readsize + 1)*sizeof(char), 0);
				if (writesize == -1) {
					perror("Error al enviar de vuelta al cliente\n");
					return -1;
				}
			}

		} while (readsize > 0);

		close(asock);
	}

	close(sd);
	return 0;
}

