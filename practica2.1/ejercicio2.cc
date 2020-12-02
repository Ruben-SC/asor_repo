#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main() {

	if(setuid(0)){
		printf("setuid(0): %s, Código de error: %i\n",strerror(errno),errno);
		exit (1);
	}
	
	return 0;
}