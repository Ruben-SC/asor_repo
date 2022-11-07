#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

	if(setuid(0)){
		perror("setuid(0)");
		exit (1);
	}
	
	return 0;
}