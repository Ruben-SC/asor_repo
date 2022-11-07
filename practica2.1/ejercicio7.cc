#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

	if(pathconf(".",_PC_LINK_MAX) == -1){
		perror("pathconf('.',_PC_LINK_MAX)");
		exit(1);
	}
	printf("Número máximo de enlaces: %ld\n", pathconf(".",_PC_LINK_MAX));

	if(pathconf(".",_PC_PATH_MAX) == -1){
		perror("pathconf('.',_PC_PATH_MAX)");
		exit(1);
	}
	printf("Tamaño máximo de una ruta: %ld\n", pathconf(".",_PC_PATH_MAX));

	if(pathconf(".",_PC_NAME_MAX) == -1){
		perror("pathconf('.',_PC_NAME_MAX)");
		exit(1);
	}
	printf("Tamaño máximo de un nombre de fichero: %ld\n", pathconf(".",_PC_NAME_MAX));

	return 0;
}