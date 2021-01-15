#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

volatile int intCont = 0;
volatile int tstpCont = 0;

void manejadorCont(int senal) {

	if (senal == SIGINT)
		intCont++;
	if (senal == SIGTSTP)
		tstpCont++;
}

int main() {

	struct sigaction manejador;

	if (sigaction(SIGINT, NULL, &manejador) == -1) {
		perror("No puede obtener el manejador SIGINT");
		return -1;
	}

	manejador.sa_handler = manejadorCont;

	if (sigaction(SIGINT, &manejador, NULL) == -1) {
		perror("No puede establecer el manejador SIGINT");
		return -1;
	}

	if (sigaction(SIGTSTP, NULL, &manejador) == -1) {
		perror("No puede obtener el manejador SIGTSTP");
		return -1;
	}

	manejador.sa_handler = manejadorCont;

	if (sigaction(SIGTSTP, &manejador, NULL) == -1) {
		perror("No puede establecer el manejador SIGTSTP");
		return -1;
	}

	sigset_t mascara;
	sigemptyset(&mascara);

	while (intCont + tstpCont < 10)
		sigsuspend(&mascara);

	printf("SIGINT recibidos: %i\n", intCont);
	printf("SIGTSTP recibidos: %i\n", tstpCont);

	return 0;
}