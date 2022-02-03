#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>

void usage(const char *program) {
    fprintf(stderr, "Usage: %s source target\n", program);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
    	usage(argv[0]);
    	return EXIT_FAILURE;
    }

    // Command line arguments
    const char *source_path = argv[1];
    const char *target_path = argv[2];

    // Open file for reading
    int source_fd = open(source_path, O_RDONLY);  
    if (source_fd < 0) {			   
    	fprintf(stderr, "filecopy: open: %s\n", strerror(errno));
    	return EXIT_FAILURE;
    }

    FILE *source_file = fdopen(source_fd, "r");	   
    if (!source_file) {
    	fprintf(stderr, "filecopy: fopen: %s\n", strerror(errno));
    	return EXIT_FAILURE;
    }

    // Open file for writing
    FILE *target_file = fopen(target_path, "w");   
    if (!target_file) {
    	fprintf(stderr, "filecopy: fopen: %s\n", strerror(errno));
    	return EXIT_FAILURE;
    }
	int bytes = 0;
    // Copy from source file to target file
    char   buffer[BUFSIZ];			 
    size_t nread;				  
    while ((nread = fread(buffer, 1, BUFSIZ, source_file)) > 0) {
		if (nread < 0) {
			fprintf(stderr, "%s: Unable to read from %s: %s\n", argv[0], source_path, strerror(errno));
			fclose(source_file);
			fclose(target_file);
    	fwrite(buffer, 1, nread, target_file);	 
		bytes += nread*sizeof(*buffer);
    }

    // Close files
    fclose(source_file);			   
    fclose(target_file);
	printf("filecopy: copied %d bytes from %s to %s\n", bytes, source_path, target_path);

    return EXIT_SUCCESS;
}
