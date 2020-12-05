#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main() {

	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long microsegundos = 1000000 * tv.tv_sec + tv.tv_usec;

	printf("Microsegundos desde el Epoch: %ld\n", microsegundos);

	return 0;
}