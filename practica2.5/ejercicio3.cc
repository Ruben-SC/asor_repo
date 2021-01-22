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

	if (argc < 4) {
		printf("Mal formato de comando, escribir: %s host port command\n",argv[0]);
		return -1;
	}

	struct addrinfo info;
	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_DGRAM;
	info.ai_flags = AI_PASSIVE;
	
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

	if (-1 == sendto(sd, argv[3], strlen(argv[3]) + 1, 0, list->ai_addr, list->ai_addrlen)) {
		perror("Error mientras se enviaba al servidor\n");
		return -1;
	}

	char buf[257];
	ssize_t readsize = recvfrom(sd, buf, 256, 0, list->ai_addr, &(list->ai_addrlen));
	if (readsize == -1) {
		perror("Error mientras se recibía del servidor\n");
		return -1;
	}

	buf[readsize] = '\0';
	printf("%s\n", buf);

	freeaddrinfo(list);

	printf("Comando %s enviado a %s : %s\n", argv[3], argv[1], argv[2]);

	close(sd);
	return 0;
}