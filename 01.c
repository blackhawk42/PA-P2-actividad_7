#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t pid;
	int niceness = nice(0);
	if (niceness < 0) {
		fprintf(stderr, "Parent had an error while nicing\n");
		exit(1);
	}

	printf("Parent has a niceness of %d\n", niceness);

	int i;
	for(i = 0; i < 5; i++) {
		pid = fork();
		if (pid < 0) {
			fprintf(stderr, "error while forking process number %d\n", i+1);
			exit(1);
		}

		if(pid == 0) {
			break;
		}
	}

	if (pid == 0) {
		pid = getpid();
		i++;
		niceness = nice( i*2 );
		if (niceness < 0) {
			fprintf(stderr, "Child %d (PID %d) had an error while nicing\n", i-1, pid);
			exit(1);
		}

		printf("Child %d (PID %d) has a niceness of %d\n", i, pid, niceness);
		exit(0);
	}


	for(i = 0; i < 5; i++) {
		wait(NULL);
	}

	printf("Parent terminating\n");

	return 0;
}
