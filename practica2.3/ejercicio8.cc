#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Mal formato de comando, escribir: %s program [args]\n",argv[0]); 
		return -1;
	}

	pid_t pid = fork();
	pid_t nsid;
	int status;

	int ncwd;
	int demon_in;
	int demon_out;
	int demon_err;
	int dup_in;
	int dup_out;
	int dup_err;
	int rc;

	switch (pid) {
		case -1:
			perror("Incapaz de hacer fork");
			return -1;
		break;

		// Hijo demonio
		case 0:
			nsid = setsid();
			if (nsid == -1) {
				perror("Incapaz de crear una nueva sesión");
				exit(-1);
			}

			ncwd = chdir("/tmp");
			if (ncwd == -1) {
				perror("Incapaz de cargar el CWD");
				exit(-1);
			}

			demon_in = open("/dev/null", O_RDONLY);
			demon_out = open("demon.out", O_WRONLY | O_CREAT);
			demon_err = open("demon.err", O_WRONLY | O_CREAT);
			if (demon_in == -1 || demon_out == -1 || demon_err == -1) {
				perror("Fallo al abrir el fichero 'in/out/err':");
				close(demon_in);
				close(demon_out);
				close(demon_err);
				exit(-1);
			}

			dup_in = dup2(demon_in, 0);
			if (dup_in == -1) {
				perror("Incapaz de duplicar el stdin:");
				exit(-1);
			}
			
			dup_out = dup2(demon_out, 1);
			if (dup_out == -1) {
				perror("Incapaz de duplicar el stdout:");
				exit(-1);
			}

			dup_err = dup2(demon_err, 2);
			if (dup_err == -1) {
				perror("Incapaz de duplicar el error stderr:");
				exit(-1);
			}

			rc = execvp(argv[1], argv + 1);
			if (rc == -1) {
				perror("Fallo de llamada al sistema");
				exit(-1);
			}

			exit(0);
		break;
		default:
			wait(&status);
			if (status == -1) {
				perror("Comando fallido");
				return -1;
			}
			printf("El comando termino de ejecutarse\n");
			return 0;
		break;
	}
}