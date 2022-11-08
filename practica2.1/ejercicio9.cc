#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {

	printf("UID real: %i\nUID efectivo: %i\n",getuid(),geteuid());
	
	return 0;
}

/***
El 'bit setuid' se asigna a ficheros ejecutables y permite, cuando un usuario lo ejecute, que el proceso adquiera los permisos del propietario del fichero ejecutado.
Se identificaría con una 's' en la lista de permisos del fichero: "-rwsr-xr–x".
***/