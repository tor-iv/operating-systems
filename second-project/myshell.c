// Victor Cox myshell.c
// basic shell with start, wait, waitfor, run, watchdog, chdir, quit functions
// need still: watchdog, chdir, pwd
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

/*
test functions from previous attempts

void stopper(int pid) {
    printf("myshell: process [%d] stopped");
}
void continuer(int pid) {
    printf("myshell: process [%d] continued");
}
void killer(int pid) {
    printf("myshell: process [%d] killed");
}
*/
int main(int argc, char* argv[]) {
    
    while (true) {
        printf("myshell> ");
        // where to store input
        char line[4096];
        fflush(stdout);
        // get input
        
        if (fgets(line, 4096, stdin) == NULL) {
            break;
        }

        // store words in command
        char* words[100] = {0};
        char* token = strtok(line, " \t\n");
        int ct = 0; // word count
        // deal with automatically hitting enter
        if (token == NULL) {
            continue;
        }
        while (token != NULL) {
            words[ct++] = strdup(token);
            token = strtok(NULL, " \t\n");
        }
        words[ct] = NULL;
        
        // run command
        if (strcmp("run", words[0]) == 0) {
            int status;
            pid_t pid = fork();
            // shift to get rid of first word / command
            for (int i = 0; i < 98; i++) {
                if (words[i+1] == 0 || (strcmp(words[i+1], "\n") == 0)) {
                    words[i] = 0;
                    break;
                }
                words[i] = words[i+1];
            }
            // pid is zero case, run exec process
            if (pid == 0) {
                int result = execvp(words[0], words);
                if (result < 0) {
                    printf("Error: %s\n", strerror(errno));
                }
                exit(1);
            }
            // fork fail
            else if (pid < 0) {
                printf("myshell: failure while forking: %s\n", strerror(errno));
                exit(1);
            }
            // other proc
            else {
                printf("myshell: proc [%d] initiated\n", pid);
            }
            // cleaning
            waitpid(pid, &status, 0);
            // status code indicates how it exits
            if (status != 0) {
                printf("myshell: process [%d] exited abnormally with signal %d: %s\n", pid, status, strsignal(status));
            } else {
                printf("myshell: process [%d] exited normally with status %d\n", pid, status);
            }
        }
        else if (strcmp("start", words[0]) == 0) {
            pid_t pid = fork();
            // drop first arg (Start)
            for (int i = 0; i < 98; i++) {
                if (words[i+1] == 0 || (strcmp(words[i+1], "\n") == 0)) {
                    words[i] = 0;
                    break;
                }
                words[i] = words[i+1];
            }
            // pid is zero
            if (pid == 0) {
                /*
                for (int i = 0; i < ct; i++) {
                    printf("Word: %s\n", words[i]);
                }
                */
                int result = execvp(words[0], words);
                if (result < 0) {
                    printf("Error: %s\n", strerror(errno));
                }
                exit(1);
            }
            // fork fail
            else if (pid < 0) {
                printf("myshell: failure while forking: %s\n", strerror(errno));
                exit(1);
            }
            // other proc
            else {
                printf("myshell: proc [%d] initiated\n", pid);
            }
        }
        // kill process
        else if (strcmp("kill", words[0]) == 0) {
            // killing failed
            if (kill(atoi(words[1]), SIGQUIT) != 0) {
                printf("myshell: couldn't kill process [%d]: %s\n", atoi(words[1]), strerror(errno));
            } else {
                printf("myshell: process [%d] killed\n", atoi(words[1]));
            }
        }
        // stop process 
        else if (strcmp("stop", words[0]) == 0) {
            // stop failed
            if (kill(atoi(words[1]), SIGSTOP) != 0) {
                printf("myshell: couldn't stop process [%d]: %s\n", atoi(words[1]), strerror(errno));
            } else {
                printf("myshell: process [%d] stopped\n", atoi(words[1]));
            }
        }
        // wait
        else if (strcmp("wait", words[0]) == 0) {
            int status, child;
            // get child status code, if equal -1 then no processes
            if ((child = wait(&status)) == -1) {
                printf("myshell: all processes gone\n");
            } else {
                // zero status means abnormal  wait
                if (status != 0) {
                    printf("myshell: error: process [%d] was abnormally, signal [%d] indicated: %s\n", child, status, strsignal(status));
                } else {
                    printf("myshell: process [%d] exited normally, status code [%d]\n", child, status);
                }
            }
        }
		// waitfor
		else if (strcmp("waitfor", words[0]) == 0) {
			int status;
			int pid = atoi(words[1]);
			// cleaning
            waitpid(pid, &status, 0);
            // status code indicates how it exits
            if (status != 0) {
                printf("myshell: process [%d] exited abnormally with signal %d: %s\n", pid, status, strsignal(status));
            } else {
                printf("myshell: process [%d] exited normally with status %d\n", pid, status);
            }
		}
		// watchdog
		else if (strcmp("watchdog", words[0]) == 0) {
			pid_t pid = fork();
			int time = atoi(words[1]);
			int status;
            // shift to get rid of first word / command
            for (int i = 1; i < 98; i++) {
                if (words[i+1] == 0 || (strcmp(words[i+1], "\n") == 0)) {
                    words[i] = 0;
                    break;
                }
                words[i] = words[i+1];
            }
            // pid is zero case, run exec process
            if (pid == 0) {
				// ERROR WITH EXEC NEED TO FIX
                int result = execvp(words[1], &words[1]);
                if (result < 0) {
                    printf("Error: %s\n", strerror(errno));
                }
                exit(1);
            }
            // fork fail
            else if (pid < 0) {
                printf("myshell: failure while forking: %s\n", strerror(errno));
                exit(1);
            }
            // other proc
            else {
				printf("myshell: proc [%d] initiated\n", pid);
				signal(SIGINT, sigaddset);
				usleep(time*(1000000));
				if (kill(pid, SIGKILL) == 0) {	
					printf("myshell: process %d exceeded the time limit, killing it...\n", pid);
				}
            }
            // cleaning
            waitpid(pid, &status, 0);
            // status code indicates how it exits
            if (status != 0) {
                printf("myshell: process [%d] exited abnoramally with signal %d: %s\n", pid, status, strsignal(status));
            } else {
                printf("myshell: process [%d] exited normally with status %d\n", pid, status);
            }
        }
        // continue
        else if (strcmp("continue", words[0]) == 0) {
            // continue failed
            if (kill(atoi(words[1]), SIGCONT) != 0) {

                printf("myshell: couldn't continue process [%d]: %s\n", atoi(words[1]), strerror(errno));
            } else {
                printf("myshell: process [%d] continued\n", atoi(words[1]));
            }
        }
		// chdir
		else if (strcmp("chdir", words[0]) == 0) {
			if (chdir(words[1]) != 0) {
				printf("Error: %s\n", strerror(errno));
			}
		}
		// pwd
		else if (strcmp("pwd",words[0]) == 0) {
			char buf[BUFSIZ];
			getcwd(buf,BUFSIZ);
			printf("%s\n",buf);
		}

        // quit, exit
        else if ((strcmp("quit", words[0]) == 0) || (strcmp("exit", words[0]) == 0)) {
            printf("Goodbye!\n");
            exit(0);
        }
        // testing mechanism
        else if (strcmp("\n", words[0]) == 0) {
            continue;
        }
        // command unknown
        else {
            printf("myshell: command not recognized \n");
        }
    }

    return 0;
}
