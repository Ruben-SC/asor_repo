#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	pid_t pid = getpid();
	pid_t pgid = getpgid(pid); 
	pid_t gid = getgid(); 
	pid_t sid = getsid(pid);
	char dir[1024];
	struct rlimit rlim;
	getrlimit(RLIMIT_NOFILE, &rlim);
	getcwd(dir, sizeof(dir));

	printf("ID, identificador del proceso: %i \n", pid);
	printf("PGID, identificador del padre del proceso: %i \n", pgid);
	printf("Identificador de grupo de procesos: %i \n", gid);
	printf("Identificador de sesión: %i \n", sid);
	printf("Número máximo de ficheros que puede abrir: %lld \n",(long long int)rlim.rlim_max);
	printf("Directorio de trabajo: %s \n", dir);
	
	return 0;
}