#include <stdlib.h>
void __attribute__((constructor)) cleanup(void){
	unsetenv("LD_PRELOAD");
	unsetenv("LD_LIBRARY_PATH");
}

long ptrace(int req, int pid, void *adr, void *dat){
	return 0;
}

