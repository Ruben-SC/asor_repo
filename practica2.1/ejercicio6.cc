#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

	if(sysconf(_SC_ARG_MAX) == -1){
		perror("sysconf(_SC_ARG_MAX)");
		exit(1);
	}
	printf("Longitud máxima de argumentos: %ld\n", sysconf(_SC_ARG_MAX));

	if(sysconf(_SC_CHILD_MAX) == -1){
		perror("sysconf(_SC_CHILD_MAX)");
		exit(1);
	}
	printf("Número máximo de hijos: %ld\n", sysconf(_SC_CHILD_MAX));

	if(sysconf(_SC_OPEN_MAX) == -1){
		perror("sysconf(_SC_OPEN_MAX)");
		exit(1);
	}
	printf("Número máximo de ficheros: %ld\n", sysconf(_SC_OPEN_MAX));

	return 0;
}