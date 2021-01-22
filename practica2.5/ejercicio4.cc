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
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Mal formato de comando, escribir: %s host port\n",argv[0]);
		return -1;
	}

	struct addrinfo info;
	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_DGRAM;
	info.ai_protocol = 0;
	info.ai_flags = AI_PASSIVE;
	info.ai_addr = NULL;
	info.ai_canonname = NULL;
	info.ai_next = NULL;

	struct addrinfo *list;
	int result = getaddrinfo(argv[1], argv[2], &info, &list);

	if (result != 0) {
		printf("Error al hacer gettinf: %s\n", gai_strerror(result));
		return -1;
	}

	int on = 1;
	int off = 0;

	int sd = socket(list->ai_family, SOCK_DGRAM, list->ai_protocol);
	if (sd == -1) {
		perror("Incapaz de abrir el socket\n");
		return -1;
	}

	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(int));
	setsockopt(sd, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(int));

	if (bind(sd, list->ai_addr, list->ai_addrlen)) {
		perror("Incapaz de hacer bind\n");
		return -1;
	}

	freeaddrinfo(list);

	char buf[2] = "";
	struct sockaddr_storage input_addr;
	socklen_t input_len = sizeof(input_addr);
	fd_set set;
	int readsize;

	while (buf[0] != 'q') {
		FD_ZERO(&set);
		FD_SET(0, &set);
		FD_SET(sd, &set);

		if (-1 == select(sd + 1, &set, NULL, NULL, NULL)) {
			perror("Error mientras se esperaba por datos\n");
			return -1;
		}

		if (FD_ISSET(sd, &set))
			readsize = recvfrom(sd, &buf, 2*sizeof(char), 0, (struct sockaddr*) &input_addr, &input_len);
		else if (FD_ISSET(0, &set))
			readsize = read(0, &buf, 2*sizeof(char));
		
		if (readsize == -1) {
			perror("Error mientras se recibía\n");
			close(sd);
			return -1;
		}
		else if (readsize == 0) {
			printf("El cliente se ha desconectado\n");
			buf[0] = 'q';
		}
		else {
			if (FD_ISSET(sd, &set)) {
				char hostname[NI_MAXHOST] = "";
				char port[NI_MAXSERV] = "";

				if (getnameinfo((struct sockaddr*) &input_addr, input_len, hostname, NI_MAXHOST, port, NI_MAXSERV, 0)) {
					perror("Error al conseguir la información desde name\n");
					return -1;
				}

				printf("Recibí %i bytes de %s:%s\n", readsize, hostname, port);
			}

			if (buf[0] == 'd') {
				time_t tiempo;
				struct tm *loctime;
				char buffer[256];
				tiempo =  time(NULL);
				loctime = localtime(&tiempo);
				strftime(buffer,256,"%Y-%m-%d", loctime);

				if (FD_ISSET(0, &set))
					printf("%s\n", buffer);
				else if (FD_ISSET(sd, &set)) {
					int writesize = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&input_addr, input_len);
					if (writesize == -1) {
						perror("Error al enviar de vuelta al cliente\n");
						return -1;
					}
				}
			} else if (buf[0] == 't') {
				time_t tiempo;
				struct tm *loctime;
				char buffer[256];
				tiempo =  time(NULL);
				loctime = localtime(&tiempo);
				strftime(buffer,256,"%r", loctime);

				if (FD_ISSET(0, &set))
					printf("%s\n", buffer);
				else if (FD_ISSET(sd, &set)) {
					int writesize = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *)&input_addr, input_len);
					if (writesize == -1) {
						perror("Error al enviar de vuelta al cliente\n");
						return -1;
					}
				}
			}
			else if (buf[0] == 'q')
				printf("Comando de finalización recibido, saliendo…\n");
			else
				printf("Comando no soportado %s\n", buf);
		}
	}

	close(sd);
	return 0;
}