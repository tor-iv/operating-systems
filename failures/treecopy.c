#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int copy_file(char *source_path, char *target_path);
int copy_dir(char *source_path, char *target_path, int bytes);

void usage(const char *program) {
	fprintf(stderr, "Usage: %s source target\n", program);
}
int copy_dir(char *source_path, char *target_path, int bytes) {
	DIR *d = opendir(source_path);
 	struct dirent *e;
    for (e = readdir(d); e; e = readdir(d)) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) {
            continue;
    	}
		if (e->d_type == DT_DIR) {
			printf("copying: %s", target_path);	
			bytes += copy_dir(source_path, target_path, 0);
		}
		else {
			bytes += copy_file(e->d_name, source_path);
		}
	}

    closedir(d);
    return bytes;
}

int copy_file(char *source_path, char *target_path) {
    // Open file for reading
    int source_fd = open(source_path, O_RDONLY);   
    if (source_fd < 0) {                           
        fprintf(stderr, "open: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    FILE *source_file = fdopen(source_fd, "r");    
    if (!source_file) {
        fprintf(stderr, "fopen: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Open file for writing
    FILE *target_file = fopen(target_path, "w");    
    if (!target_file) {
        fprintf(stderr, "fopen: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    // Copy from source file to target file
    char   buffer[BUFSIZ];                          
    size_t nread;  
    int bytes = 0;                                
    while ((nread = fread(buffer, 1, BUFSIZ, source_file)) > 0) {
        fwrite(buffer, 1, nread, target_file);      
        bytes += nread;
    }

    // Close files
    fclose(source_file);                           
    fclose(target_file);

    return bytes;
}
int main(int argc, char *argv[]) {
    
	if (argc != 3) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}
    char path[BUFSIZ];
	const char *source_path = argv[1];
	const char *og_path = argv[1];
	const char *target_path = argv[2];
	int bytes = 0;
    /* Open directory for reading */
    DIR *d = opendir(path);
    if (!d) {
		bytes = copy_file(source_path, target_path);
		printf("%s -> %s", source_path, target_path);
		printf("file copy%d\n",bytes);
		return EXIT_SUCCESS;
    }

    /* Iterate through directory entries */
    struct dirent *e;
    for (e = readdir(d); e; e = readdir(d)) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) {
            continue;
        }
		if (e->d_type == DT_DIR) {
			printf("copying: %s", target_path);	
			bytes += copy_dir(source_path, target_path, 0);
		}
	}

    closedir(d);
    return EXIT_SUCCESS;
}


