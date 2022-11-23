#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv){

	int file;

	if(argv[1]){// con nombre escogido
		file = open(argv[1], O_CREAT, 0645);
	}
	else{// por defecto
		file = open("ej5", O_CREAT, 0645);
	}

	return 0;
}