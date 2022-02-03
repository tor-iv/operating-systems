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

// global vairables
int bytes, dcnt, fcnt = 0;
char *source, *target, *prog;

// define functions
int determine(char *src, char *trgt);
int fileCopy(char *src, char *trgt);
int dirCopy(char *src, char *trgt);

// use stat system call to determine if directory file or soemthign else
int determine(char *src, char *trgt) {	
	struct stat info;
	if(stat(src, &info)){
		fprintf(stderr, "%s:Unable to copy %s\n", prog, src);
		return EXIT_FAILURE;
	}
	// if directory call dirCopy 
	if(S_ISDIR(info.st_mode)){
		dcnt += 1;
		return dirCopy(src, trgt);
	}
	// if file call fileCopy
	else if(S_ISREG(info.st_mode)){
		fcnt += 1;
		return fileCopy(src, trgt);
	}
	// otherwise throw error
	else{
		fprintf(stderr, "%s:Unable to copy file %s\n", prog, source);
		return EXIT_FAILURE;
	}

}

// filecopy
int fileCopy(char *src, char *trgt){
	char buffer[BUFSIZ];
	int nread;

	//open file for reading
	int srcfd = open(src, O_RDONLY);
	if (srcfd < 0){
		fprintf(stderr, "%s: Unable to open %s: %s\n", prog, target, strerror(errno));
		return EXIT_FAILURE;
	}
	// open target file with correct permissions
	int trgtfd = open(trgt, O_CREAT|O_WRONLY, 0666);
	if (trgtfd < 0){
		fprintf(stderr, "%s:Unable to open %s: %s\n", prog, target, strerror(errno));
		close(srcfd);
		return EXIT_FAILURE;
	}
	// read through source file copy it over to target file
	while((nread = read(srcfd, buffer, BUFSIZ)) > 0){

		if (nread < 0){
			fprintf(stderr, "%s: Unable to read from %s: %s\n", prog, source, strerror(errno));
			close(srcfd);
			close(trgtfd);
			return EXIT_FAILURE;
		}
		// increment bytes
		bytes += nread*sizeof(*buffer);
	}
	//output
	printf("%s  -> %s\n", src, trgt);
	return EXIT_SUCCESS;

}

// dircopy
int dirCopy(char *src, char *trgt){
	// open directory
	DIR *srcdir = opendir(src);
	if (!srcdir){
		fprintf(stderr, "%s: Unable to open directory %s : %s\n", prog, src, strerror(errno));
		return EXIT_FAILURE;
	}
	// create new directory
	int result = mkdir(trgt, 0777);
	if (result < 0){
		fprintf(stderr, "%s: Unable to create directory %s : %s\n", prog, trgt, strerror(errno));
		return EXIT_FAILURE;
	}
	// output progress
	printf("%s  -> %s\n", src, trgt);
	// iterate through directory and with excpetion of parent and itself make copy by first calling determine
	struct dirent *d;
	while ((d = readdir(srcdir))){
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) {
			continue;
		}

		char newsource[BUFSIZ];
		char newtarget[BUFSIZ];
		// create new path
		snprintf(newsource, BUFSIZ, "%s/%s", src, d->d_name);
		snprintf(newtarget, BUFSIZ, "%s/%s", trgt, d->d_name);

		determine(newsource, newtarget);
	}


	closedir(srcdir);
	return 0;
}

// main
int main(int argc, char *argv[]){
	// check arguemnts
	if(argc != 3){
		printf("Usage: %s SOURCE TARGET\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct stat info;
	if(stat(target, &info) == 0){
		fprintf(stderr, "%s:Target %s already exists\n", prog, target);
		return EXIT_FAILURE;
	}

	prog = argv[0];
	source = argv[1];
	target = argv[2];

	if(determine(source, target)) {
		return EXIT_FAILURE;
	}
	// output
	printf("%s copied %d directories, %d files, and %d bytes from %s to %s\n", prog, dcnt, fcnt, bytes, source, target);
	return EXIT_SUCCESS;
}


