#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main() {

	if (-1 == mkfifo("tuberia1", 0644)) {
		perror("No se puede crear la primera tubería");
		return -1;
	}

	if (-1 == mkfifo("tuberia2", 0644)) {
		perror("No se puede crear la segunda tubería");
		return -1;
	}

	char buffer[257];
	int nbuff = 0;

	int tuberia1 = open("tuberia1", O_RDONLY | O_NONBLOCK);
	if (tuberia1 == -1) {
		perror("No se puede abrir la primera tubería");
		close(tuberia1);
		return -1;
	}

	int tuberia2 = open("tuberia2", O_RDONLY | O_NONBLOCK);
	if (tuberia2 == -1) {
		perror("No se puede abrir la segunda tubería");
		close(tuberia1);
		close(tuberia2);
		return -1;
	}

	int seleccion, tub_actual;
	do {
		fd_set tuberias;
		FD_ZERO(&tuberias);
		FD_SET(tuberia1, &tuberias);
		FD_SET(tuberia2, &tuberias);

		seleccion = select((tuberia1 < tuberia2) ? tuberia2 + 1 : tuberia1 + 1, &tuberias, NULL, NULL, NULL);

		if (seleccion > 0) {
	
			int ntub = 0;
			if (FD_ISSET(tuberia1, &tuberias)) {
				ntub = 1;
				tub_actual = tuberia1;
			}
			else if (FD_ISSET(tuberia2, &tuberias)) {
				ntub = 2;
				tub_actual = tuberia2;
			}

			ssize_t tam = 256;
			while (tam == 256) {

				tam = read(tub_actual, buffer, 256);

				if (tam == -1) {
					perror("Incapaz de leer");
					close(tuberia1);
					close(tuberia2);
					return -1;
				}

				buffer[tam] = '\0';
				printf("Tuberia %i: %s", ntub, buffer);
			}

			if (tam != 256 && ntub == 1) {
				close(tuberia1);
				tuberia1 = open("tuberia1", O_RDONLY | O_NONBLOCK);

				if (tuberia1 == -1) {
					perror("No se puede abrir la primera tubería");
					close(tuberia1);
					close(tuberia2);
					return -1;
				}
			}

			else if (tam != 256 && ntub == 2) {
				close(tuberia2);
				tuberia2 = open("tuberia2", O_RDONLY | O_NONBLOCK);

				if (tuberia2 == -1) {
					perror("No se puede abrir la segunda tubería");
					close(tuberia1);
					close(tuberia2);
					return -1;
				}
			}
		}

	} while (seleccion != -1);

	perror("Error en la selección");
	close(tuberia1);
	close(tuberia2);

	return -1;
}