#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {

	if (argc < 2)
		return -1;

	int file = open("./tuberia", O_WRONLY);
	if (file == -1) {
		perror("No se puede abrir la tubería");
		close(file);
		return -1;
	}

	size_t arglen = strlen(argv[1]);
	ssize_t wr = write(file, argv[1], arglen);
	if (wr == -1) {
		perror("No se puede escribir en la tubería");
		close(file);
		return -1;
	}

	close(file);
	return 0;
}