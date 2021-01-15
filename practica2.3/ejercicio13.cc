#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

volatile int stop = 0;

void manejador(int senal) {
	if (senal == SIGUSR1)
		stop = 1;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		printf("Mal formato, escriba: %s 'segundos'\n",argv[0]);
		return -1;
	}

	sigset_t mascara;
	sigemptyset(&mascara);
	sigaddset(&mascara, SIGUSR1);

	if (sigprocmask(SIG_UNBLOCK, &mascara, NULL)) {	
		perror("Incapaz de desbloquear SIGUSR1");
		return -1;
	}

	struct sigaction handler;

	if (sigaction(SIGUSR1, NULL, &handler) == -1) {
		perror("No puede obetener el manejador SIGUSR1");
		return -1;
	}

	handler.sa_handler = manejador;

	if (sigaction(SIGUSR1, &handler, NULL) == -1) {
		perror("no puede establecer el manejador SIGUSR1");
		return -1;
	}

	sleep(atoi(argv[1]));

	if (stop == 1) {
		printf("Autoeliminación parada\n");
		return 0;
	}

	if (unlink(argv[0]) == -1) {
		perror("Incapaz de eliminarse a sí mismo");
		return -1;
	}
	else
		printf("Se ha eliminado a sí mismo\n");

	exit(0);
	return 0;
}