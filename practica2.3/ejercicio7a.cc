#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	
	if(argc < 2)	{
		printf("Mal formato de comando, escribir: %s 'comando' <argv1> <argv2> ...",argv[0]);
		exit(EXIT_FAILURE);
	}

	int i;
	char comando[255];
	strcpy(comando, argv[1]);
	for(i= 2; i < argc; i++) {
		strcat(comando, " ");
		strcat(comando,argv[i]);
	}
	
	system(comando);

	printf("El comando termino de ejecutarse\n");

	return 0;
}