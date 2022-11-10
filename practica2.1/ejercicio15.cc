#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

int main() {

	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	char buffer[50];

	setlocale(LC_TIME,"es_ES");
	strftime(buffer, sizeof(buffer), "%A, %d de %B de %Y, %H:%M", tm);

	printf("Fecha: %s\n", buffer);
	
	return 0;
}