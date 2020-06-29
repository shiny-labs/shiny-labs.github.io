#include <sys/ptrace.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include "stub.h"

int anti_ptrace(void) {
	pid_t pid;
	pid_t ppid;
	int status;
	int success = 1;
	int res = 0;
	pid = fork();
	if(pid == 0){
		/* wait for signal from parent */
		wait(NULL);
		ppid = getppid();
		res = ptrace(PTRACE_ATTACH, ppid, 0, 0);
		if(res == 0){
			/* wait for parent to be ready for ptrace */
			waitpid(ppid, NULL, 0);
			ptrace(PTRACE_CONT, ppid, 0, 0);
			ptrace(PTRACE_DETACH, ppid, 0, 0);
		}
		exit(res);
	} else if(pid > 0) {
		/* prepare to be ptraced */
#ifdef PR_SET_PTRACER
		prctl(PR_SET_PTRACER, pid);
#endif
		prctl(PR_SET_DUMPABLE, 1);
		/* instruct child to continue */
		kill(pid, SIGCONT);
		/* wait for child to exit */
		waitpid(pid, &status, 0);
		prctl(PR_SET_DUMPABLE, 0);
		success = WEXITSTATUS(status);
	}
	return success;
}

int anti_ld_env(char **envp){
	char **p;
	for (p = envp; *p != NULL; p++) {
		if ((*p)[0] == 'L' && (*p)[1] == 'D' && (*p)[2] == '_')
			return 1;
	}
	return 0;
}

int anti_dumpable(void){
	if (prctl(PR_SET_DUMPABLE, 0) < 0)
		return 1;
	return 0;
}

int main(int argc, char* argv[], char** envp){
	if(anti_dumpable() || anti_ptrace() || anti_ld_env(envp)){
		printf("Nope!\n");
		return 1;
	}
	printf("Okay.\n");
	return 0;
}

