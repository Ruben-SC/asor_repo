#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char ** argv) {

	if(argc != 2) {
		fprintf(stderr, "Mal formato de comando\nEscribir: %s <ruta fichero>\n", argv[0]);
		return -1;
	}

	int file;

	file = open(argv[1], O_CREAT | O_RDWR, 0777);
	if (file == -1) {
		printf("Error al abrir el fichero, %i - %s\n", errno, strerror(errno));
		return -1;
	}

	struct flock cerrojo;
	cerrojo.l_type = F_WRLCK;
	cerrojo.l_whence = SEEK_SET;
	cerrojo.l_start = 0;
	cerrojo.l_len = 0;
	cerrojo.l_pid = getpid();

	if (fcntl(file, F_GETLK, &cerrojo) == -1) {
		printf("Error al obtener el bloqueo, %i - %s\n", errno, strerror(errno));
		close(file);
		return -1;
	}

	if(cerrojo.l_type == F_WRLCK){
		printf("El proceso %ld ya tiene bloqueo de escritura\n", cerrojo.l_pid);
		printf("%i - %s\n", errno, strerror(errno));
		close(file);
		return -1;
	}
	else if (cerrojo.l_type == F_RDLCK) {
	        printf("El proceso %ld ya tiene bloqueo de lectura\n", cerrojo.l_pid);
		printf("%i - %s\n", errno, strerror(errno));
		close(file);
		return -1;
	}

	if (fcntl(file, F_SETLK, &cerrojo) == -1) {
		if (errno == EACCES || errno == EAGAIN) {
			printf("Está bloqueado por otro proceso\n");
			printf("%i - %s\n", errno, strerror(errno));
			close(file);
			return -1;
		}
		else {
			printf("Error al colocar el bloqueo, %i - %s\n", errno, strerror(errno));		
			close(file);
			return -1;
		}
	}
	else {
		time_t tiempo;
		tiempo = time(NULL);

		struct tm * lt;
		lt = localtime(&tiempo);

		int MAX = 100;
		char buf[MAX];
		char * format = "Tiempo %T";
	
		int length = strftime(buf, MAX, format, lt);
		if(length == 0){
			puts("No pudo dar formato al tiempo");
			exit(EXIT_FAILURE);
		}
		write(file, buf, length);

		cerrojo.l_type = F_UNLCK;
		cerrojo.l_whence = SEEK_SET;
		cerrojo.l_start = 0;
		cerrojo.l_len = 0;
		if (fcntl(file, F_SETLK, &cerrojo) == -1){
			printf("Error al quitar el bloqueo, %i - %s\n", errno, strerror(errno));
			close(file);
			return -1;
		}
	}
	close(file);
	return 0;
}