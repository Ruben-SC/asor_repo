#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int showProcessInfo(char *id) {

	if (id == NULL) {
		return -1;
	}

	pid_t pid = getpid();
	gid_t gid = getgid();
	pid_t sid = getsid(pid);

	struct rlimit rlim;
	int rc = getrlimit(RLIMIT_NOFILE, &rlim);
	
	if (rc == -1) {
		perror("No se pueden obtener los límites de recursos");
		return -1;
	}

	char *path = (char*)malloc(sizeof(char)*(4096 + 1));
	char *rpath = getcwd(path, 4096 + 1);

	if (rpath == NULL) {
		perror("No se puede obtener la ruta CWD");
		free(path);
		return -1;
	}

	printf("[%s] PID, identificador de proceso: %i\n", id, pid);
	printf("[%s] GID, identificador de grupo de proceso: %i\n", id, gid);
	printf("[%s] SID, identificador de sesión: %i\n", id, sid);
	printf("[%s] Máximo de ficheros: %ld\n", id, rlim.rlim_max);
	printf("[%s] CWD, ruta del proceso: %s\n", id, path);
		
	free (path);
	return 0;
}

int main() {
	pid_t pid = fork();
	pid_t nsid;
	
	switch (pid) {
		case -1:
			perror("Incapaz de hacer fork");
			return -1;
		break;

		// Hijo
		case 0: 
			nsid = setsid();
			if (nsid == -1) {
				perror("Incapaz de crear una nueva sesión");
				exit(-1);
			}

			if ((chdir("/tmp")) == -1) {
				perror("Incapaz de cargar el CWD");
				exit(-1);
			}

			showProcessInfo("Hijo");
			exit(0);
		break;

		// Padre
		default:
			showProcessInfo("Padre");
			int status;
			wait(&status);
			if (status == -1) {
				perror("El proceso Hijo falló");
				return -1;
			}
	}

	return 0;
}