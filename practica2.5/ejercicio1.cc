#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	struct addrinfo info;
	struct addrinfo *res, *ptr;

	memset(&info, 0, sizeof(struct addrinfo));

	info.ai_flags = AI_PASSIVE;
	info.ai_family = AF_UNSPEC;

	int result = getaddrinfo(argv[1], NULL, &info, &res);

	if(result != 0) {
		printf("Error al hacer getaddrinfo: %s\n", gai_strerror(result));
		exit (-1);
	}

	for(ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		char host[NI_MAXHOST];
		getnameinfo(ptr->ai_addr, ptr->ai_addrlen, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		printf("%s\t%i\t%i\n", host, ptr->ai_family, ptr->ai_socktype);
	}

	freeaddrinfo(res);

	return 0;
}
