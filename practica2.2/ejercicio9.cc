#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {

	struct stat sb;

	if(stat(argv[1], &sb) == -1) {
		perror("stat()");
		return -1;
	}
	
	printf("File type: ");

	switch (sb.st_mode & S_IFMT) {
       		case S_IFREG:
			printf("regular file\n");
			break;
        	case S_IFDIR:
			printf("directory\n");
			break;
        	case S_IFCHR:
			printf("character device\n");
			break;
       		case S_IFBLK:
			printf("block device\n");
			break;
        	case S_IFIFO:
			printf("FIFO/pipe\n");
			break;
       		case S_IFLNK:
			printf("symbolic link\n");
			break;
           	case S_IFSOCK:
			printf("socket\n");
			break;
           	default:
			printf("unknown\n");
			break;
	}

        printf("Inode: %ld\n", (long) sb.st_ino);

        printf("Mode: %lo (octal)\n",(unsigned long) sb.st_mode);

        printf("Links: %ld\n", (long) sb.st_nlink);
        printf("Ownership: Uid=%ld   Gid=%ld\n",(long) sb.st_uid, (long) sb.st_gid);

        printf("I/O Block: %ld\n",(long) sb.st_blksize);
        printf("Size: %lld\n",(long long) sb.st_size);
        printf("Blocks: %lld\n",(long long) sb.st_blocks);

	printf("Access: %i\n", ctime(&sb.st_atime));
	printf("Modify: %i\n", ctime(&sb.st_mtime));
	printf("Change: %i\n", ctime(&sb.st_ctime));

	return 0;
}