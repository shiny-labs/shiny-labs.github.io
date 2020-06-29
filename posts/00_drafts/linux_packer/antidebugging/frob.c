#include <stdio.h>
#include <string.h>
#include <malloc.h>
FILE* fd;
char *block;
int main(int argc, char* argv[]){
	size_t start, end;

	if(argc != 4){
		printf("Usage: %s <target> <start> <end>\n", argv[0]);
		return 1;
	}
	start = strtol(argv[2], NULL, 16);
	end = strtol(argv[3], NULL, 16);
	if(!(fd = fopen(argv[1], "r+"))){
		perror("Opening target");
		return 1;
	}
	if(fseek(fd, start, SEEK_SET) != 0){
		perror("Seeking to start of area");
		return 1;
	}
	if(!(block = malloc(end-start))){
		perror("Malloc()ing block");
		return 1;
	}
	if(fread(block, end-start, 1, fd) != 1){
		perror("Reading block");
		return 1;
	}
	memfrob(block, end-start);
	if(fseek(fd, start, SEEK_SET) != 0){
		perror("Seeking to start of area");
		return 1;
	}
	if(fwrite(block, end-start, 1, fd) != 1){
		perror("Writing block to area");
		return 1;
	}
	printf("Frobbed area between 0x%x and 0x%x\n", (unsigned int) start, (unsigned int) end);
	return 0;
}

void __attribute__((destructor)) cleanup(void){
	if(block){
		free(block);
	}
	if(fd){
		fclose(fd);
	}
}

