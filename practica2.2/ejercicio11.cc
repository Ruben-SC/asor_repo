#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
	struct stat sb;
	char slink[255];
	char hlink[255];

	if (argc != 2) {
		fprintf(stderr, "Mal formato de comando\nEscribir: %s <ruta fichero>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &sb) == -1) {
		perror("stat()");
		exit(EXIT_FAILURE);
	}

	strcpy(slink, argv[1]);
	strcat(slink,".sym");
	strcpy(hlink, argv[1]);
	strcat(hlink,".hard");
	switch (sb.st_mode & S_IFMT) {
		case S_IFREG:
			if(symlink(argv[1], slink) == -1) {
				perror("symlink()");
				exit(EXIT_FAILURE);
			}

			if(link(argv[1], hlink) == -1) {
				perror("link()");
				exit(EXIT_FAILURE);	
			}
		break;
		default:
			printf("El fichero %s no es un fichero regular\n",argv[1]);                
		break;
	}

	return 0;
}