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

	if (argc < 3) {
		printf("Mal formato de comando, escribir: %s host port\n",argv[0]);
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
	int result = getaddrinfo(argv[1], argv[2], &info, &list);

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

	if (connect(sd, list->ai_addr, list->ai_addrlen)) {
		perror("Incapaz de conectar\n");
		close(sd);
		return -1;
	}

	freeaddrinfo(list);

	char buf[257] = "";
	ssize_t readsize, writesize;

	while (strcmp("Q\n", buf)) {
		readsize = read(0, buf, 256*sizeof(char));
		if (readsize == -1) {
			perror("Error mientras se leía\n");
			close(sd);
			return -1;
		}

		buf[readsize] = '\0';
		writesize = send(sd, buf, (readsize + 1)*sizeof(char), 0);
		if (writesize == -1) {
			perror("Error mientras se enviaba\n");
			close(sd);
			return -1;
		}

		readsize = recv(sd, buf, 256*sizeof(char), 0);
		if (readsize == -1) {
			perror("Error mientras se leía\n");
			close(sd);
			return -1;
		}
		buf[readsize] = '\0';
		printf("%s", buf);
	}

	close(sd);
	return 0;
}