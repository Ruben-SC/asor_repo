#include <stdio.h>
#include <time.h>

int main() {

	time_t segundos = time(NULL);

	printf("Segundos desde el Epoch: %ld\n", segundos);

	return 0;
}