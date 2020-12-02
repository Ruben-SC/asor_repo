#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main() {

	for(int i=0; i <= 255; i++){
		if(!strstr(strerror(i), "Unknown error")){
			printf("Descripción de error: %s, Código de error: %i\n",strerror(i),i);
		}
	}
	
	return 0;
}