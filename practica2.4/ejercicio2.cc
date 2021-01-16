#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main() {

	int p_h[2];
	int h_p[2];

	if (-1 == pipe(p_h) | -1 == pipe(h_p)) {
		perror("No ha podido crear la tuberia");
		return -1;
	}

	pid_t pid = fork();
	switch (pid) {
		case -1:
			perror("Incapaz de hacer fork");
			exit(-1);
		break;

		case 0:
			close(p_h[1]);
			close(h_p[0]);

			char mensaje_recibido[257];
			char mensaje_padre[1];
			mensaje_padre[1] = {'l'};

			int i;
			for (i = 0; i < 10; i++) {
				ssize_t h_size = read(p_h[0], mensaje_recibido, 256);
				if (h_size == -1) {
					perror("[Hijo] Error al leer la tubería");
					exit(-1);
				}
				mensaje_recibido[h_size] = '\0';

				printf("[Hijo] Mensaje: %s", mensaje_recibido);
				sleep(1);

				if (i == 9)
					mensaje_padre[0] = 'q';

				h_size = write(h_p[1], mensaje_padre, 1);
				if (h_size == -1) {
					perror("[Hijo] Error al escribir en la tubería");
					exit(-1);
				}
			}

			close(p_h[0]);
			close(h_p[1]);
			exit(0);
		break;

		default:
			close(p_h[0]);
			close(h_p[1]);

			char mensaje_usuario[257];
			char mensaje_hijo[1] = {'l'};
			
			while (mensaje_hijo[0] != 'q') {
				printf("[Padre] Introducir nuevo mensaje:\n");

				ssize_t p_size = read(0, mensaje_usuario, 256);
				if (p_size == -1) {
					perror("[Padre] Error al leer del stdin");
					exit(-1);
				}

				mensaje_usuario[p_size] = '\0';
				p_size = write(p_h[1], mensaje_usuario, p_size + 1);
				if (p_size == -1) {
					perror("[Padre] Error al escribir en la tubería");
					exit(-1);
				}

				while (mensaje_hijo[0] != 'l' && mensaje_hijo[0] != 'q') {
					p_size = read(h_p[0], mensaje_hijo, 1);
					if (p_size == -1) {
						perror("[Padre] Error mientras lee de la tubería");
						exit(-1);
					}
				}
			}

			close(p_h[1]);
			close(h_p[0]);
			exit(0);
	}

	return 0;
}