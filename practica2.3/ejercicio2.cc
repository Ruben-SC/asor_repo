#include <sched.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	
	int politica;
	struct sched_param parametros;
	
	politica = sched_getscheduler(0);
	sched_getparam(0, &parametros);
	
	int max_prioridad = sched_get_priority_max(politica);
	int min_prioridad = sched_get_priority_min(politica);
	printf("Proceso actual: %i\n",getpid());
	if(politica == 0) {
		printf("Política del proceso: SCHED_OTHER\nPrioridad: %i\n",parametros.sched_priority);
	} else if(politica == 1) {
		printf("Política del proceso: SCHED_FIFO\nPrioridad: %i\n",parametros.sched_priority);
	} else if(politica == 2) {
		printf("Política del proceso: SCHED_RR\nPrioridad: %i\n",parametros.sched_priority);
	}

	printf("Prioridad máxima: %i\nPrioridad minima: %i\n", max_prioridad, min_prioridad);

	return 0;
}