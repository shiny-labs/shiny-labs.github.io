#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <signal.h>
#include "stub.h"

inline char* random_string(void) {
	static unsigned char s[15];
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	static int i;

	for (i = 0; i < 4 + rand() % 10; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	for (; i < 15; ++i) {
		s[i] = '\x00';
	}

	return s;
}

void sighandler(int sig){}

inline void migrate(void){
	pid_t pid;
	pid = fork();
	kill(0, SIGSEGV);
	prctl(PR_SET_NAME, random_string());
		setsid();
	if(pid > 0){
		exit(rand());
	} else {
	}
}

int main(int argc, char* argv[]){
	argv[0] = NULL;
	int i = 1;
	unsigned char current_name[15];
	signal(SIGSEGV, sighandler);
	printf("Try to find me!\n");
	/* will run for 10 seconds */
	while(i < 50){
		usleep(200000); /* 5 times per second - tune this */
		migrate();
		i++;
	}
	prctl(PR_GET_NAME, current_name);
	printf("You can't find me, huh? I was hiding in pid %i with processname %s :)\n", getpid(), current_name);
	return 0;
}

