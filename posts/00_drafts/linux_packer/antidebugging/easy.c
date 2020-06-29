#include <stdio.h>
#include <string.h>

char sekrit[20] = "|OXSuyOIXO^";
int haxxor = 0;
void show_pw(char* foo){
	haxxor = 1;
	memfrob(foo, strlen(foo));
	printf("The password is %s\n", foo);
}
int main(int argc, char** argv){
	printf("Hello, HITR2NLDB!\n");
	if(haxxor){
		show_pw(sekrit);
	} else {
		kill(NULL, 9);
	}
	return 0;
}

