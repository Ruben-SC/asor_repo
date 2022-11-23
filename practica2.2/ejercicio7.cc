#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	mode_t mask;
	mask = umask(0027);
	int file;

	if((file = open("ej7", O_CREAT, 0666)) == -1){
		perror("open()");
		exit (1);
	}
	
	umask(mask);

	return 0;
}