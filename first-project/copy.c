// include libraries
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

//usage
void usage(const char*program) {
	fprintf(stderr, "Usage: %s source, target\n", program);
}

// main file could seperate into 2 but kept as one because didnt feel it was necessary
int main(int argc, char *argv[]){
	// check if correct amount of commands if not through usage
	if (argc != 3) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
	// declare variables
	char buffer[BUFSIZ];
	int nread;
	int bytes = 0;
	char *prog = argv[0];
	char *src = argv[1];
	char *trgt = argv[2];

	//open file for reading
	int srcfd = open(src, O_RDONLY);
	if (srcfd < 0){
		fprintf(stderr, "%s: Unable to open %s: %s\n", prog, trgt, strerror(errno));
		return EXIT_FAILURE;
	}

	//open file for writing
	int trgtfd = open(trgt, O_CREAT|O_WRONLY, 0666);
	if (trgtfd < 0){
		fprintf(stderr, "%s:Unable to open %s: %s\n", prog, trgt, strerror(errno));
		close(srcfd);
		return EXIT_FAILURE;
	}

	// read through source file copy it over to target file
	while((nread = read(srcfd, buffer, BUFSIZ)) > 0){

		if (nread < 0){
			fprintf(stderr, "%s: Unable to read from %s: %s\n", prog, src, strerror(errno));
			close(srcfd);
			close(trgtfd);
			return EXIT_FAILURE;
		}
		bytes += nread*sizeof(*buffer);
	}
	//output
	printf("filecopy: copied %d bytes from %s to %s\n", bytes, src, trgt);
	return EXIT_SUCCESS;

}

