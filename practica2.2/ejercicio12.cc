#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {

	int file = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	printf("Hola mundo - ejercicio 12");

	close(1);
	dup(file);

	return 0;
}