#include <sys/utsname.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	
	struct utsname sistema;

	if(uname(&sistema) == -1){
		perror("uname(&sistema)");
		exit(1);
	}

	printf("Sistema operativo: %s \nNombre de red interna: %s \nLanzamiento del sistema operativo: %s \nVersión del sistema operativo: %s \nIdentificador del hardware: %s \nNombre de dominio: %s\n", sistema.sysname, sistema.nodename, sistema.version, sistema.release, sistema.machine, sistema.domainname);

	return 0;
}