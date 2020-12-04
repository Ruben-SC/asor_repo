#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main() {
	uid_t uidr = getuid();
	uid_t uide = geteuid();
	printf("UID real: %i\nUID efectivo: %i\n\n",uidr,uide);

	struct passwd *datos_usuarioReal = getpwuid(uidr);
	struct passwd *datos_usuario = getpwuid(uide);

	printf("Datos del usuario real\nNombre de usuario: %s \nDirectorio Home: %s \nDescripción del usuario: %s\n\n", datos_usuarioReal->pw_name, datos_usuarioReal->pw_dir, datos_usuarioReal->pw_gecos);

	printf("Datos del usuario\nNombre de usuario: %s \nDirectorio Home: %s \nDescripción del usuario: %s\n",  datos_usuario->pw_name, datos_usuario->pw_dir, datos_usuario->pw_gecos); 

	return 0;
}