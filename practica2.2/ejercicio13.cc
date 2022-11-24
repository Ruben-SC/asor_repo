#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {

	int file = open(argv[1], O_CREAT | O_WRONLY | O_APPEND , 0666);

	close(1);
	close(2);
	dup2(file, 1);
	dup2(file, 2);

	printf("Hola mundo - ejercicio 13");
	if(setuid(0) == -1) {
		perror("setuid()");
	}

	return 0;
}