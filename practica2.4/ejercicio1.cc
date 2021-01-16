#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	if (argc != 5) {
		printf("Mal formato de comando, escribir: %s comando1 argumento1 | comando2 argumento2\n",argv[0]);
		return -1;
	}

	int tuberia[2];
	if (pipe(tuberia) == -1) {
		perror("No ha podido crear la tuberia");
		return -1;
	}

	pid_t pid = fork();
	int nfd;

	switch (pid) {
		case -1:
			perror("Incapaz de hacer");
			exit(-1);
		break;

		case 0:
			close(tuberia[1]);
			nfd = dup2(tuberia[0], 0);
			close(tuberia[0]);
			execlp(argv[3], argv[3], argv[4], NULL);
		break;

		default:
			close(tuberia[0]);
			nfd = dup2(tuberia[1], 1);
			close(tuberia[1]);
			execlp(argv[1], argv[1], argv[2], NULL);
	}

	return 0;
}