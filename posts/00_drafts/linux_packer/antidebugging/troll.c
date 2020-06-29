#include <stdio.h>
#include <string.h>
#include <linux/elf.h>
#include <malloc.h>

char *strtab;
FILE *fd;
Elf32_Ehdr elf_header32;
Elf64_Ehdr elf_header64;
Elf32_Shdr section_headers_string_table32, section_header32;
Elf64_Shdr section_headers_string_table64, section_header64;

#define elf_header(bits) elf_header##bits
#define section_headers_string_table(bits) section_headers_string_table##bits
#define section_header(bits) section_header##bits

#define get_section_by_index(bits, header, index) get_section_by_index##bits(header, index)
#define get_section_by_index_func(bits) \
Elf##bits##_Shdr get_section_by_index##bits(Elf##bits##_Ehdr header, int index){ \
	Elf##bits##_Shdr section;\
	if(fseek(fd, header.e_shoff + (index * header.e_shentsize), SEEK_SET) == 0){\
		fread(&section, sizeof(section), 1, fd) == sizeof(section);\
	}\
	return section;\
}
get_section_by_index_func(32)
get_section_by_index_func(64)

#define get_section_name(bits, name) get_section_name##bits(name)
#define get_section_name_func(bits) \
char* get_section_name##bits(Elf##bits##_Word sh_name){ \
	return strtab+sh_name;\
}
get_section_name_func(32)
get_section_name_func(64)

char *wipestring1 = "TRO";
char *wipestring2 = "LO";

#define wipe(length) \
if(length){\
	int i;\
	if(fwrite(wipestring1, strlen(wipestring1), 1, fd) != 1){\
		perror("Wiping (part 1)");\
		return 1;\
	}\
	for(i = 0; i < (length-strlen(wipestring1))/strlen(wipestring2); i++){\
		if(fwrite(wipestring2, strlen(wipestring2), 1, fd) != 1){\
			perror("Wiping  (part 2)");\
			return 1;\
		}\
	}\
}

#define troll(bits) \
if(elf_header(bits).e_version == 31337){;\
	printf("This ELF was already trollfuscated\n");\
	return 1;\
}\
section_headers_string_table(bits) = get_section_by_index(bits,elf_header(bits), elf_header(bits).e_shstrndx);\
strtab = calloc(1, section_headers_string_table(bits).sh_size+1);\
if(fseek(fd, section_headers_string_table(bits).sh_offset, SEEK_SET) != 0){\
	perror("Seeking strings table\n");\
	return 1;\
}\
if(fread(strtab, section_headers_string_table(bits).sh_size, 1, fd) != 1){\
	perror("Reading strings table\n");\
	return 1;\
}\
for(i = 0; i < elf_header(bits).e_shnum; i++){\
	section_header(bits) = get_section_by_index(bits, elf_header(bits), i);\
	section_name = get_section_name(bits, section_header(bits).sh_name);\
	if(   (strcmp(section_name, ".comment") == 0)\
	   || (strcmp(section_name, ".strtab") == 0)\
	   || (strcmp(section_name, ".shstrtab") == 0)\
	   || (strcmp(section_name, ".note.ABI-tag") == 0)\
	   || (strcmp(section_name, ".note.gnu.build-id") == 0)){\
		printf("Wiping section: %s\n", section_name);\
		if(fseek(fd, section_header(bits).sh_offset, SEEK_SET) != 0){\
			perror("Seeking to section");\
			return 1;\
		}\
		wipe(section_header(bits).sh_size)\
	}\
}\
printf("Wiping section header table\n");\
if(fseek(fd, elf_header(bits).e_shoff, SEEK_SET) != 0){\
	perror("Seeking to start of file\n");\
	return 1;\
}\
wipe(elf_header(bits).e_shnum*elf_header(bits).e_shentsize)\
elf_header(bits).e_version  = 31337;\
elf_header(bits).e_shnum    = 31337;\
elf_header(bits).e_flags    = 31337;\
elf_header(bits).e_shstrndx = 0;\
strncpy((((void*) &elf_header(bits))+1), "ELFSAREUBER1337", 15);\
printf("Obfuscating header\n");\
if(fseek(fd, 0, SEEK_SET) != 0){\
	perror("Seeking to start of file\n");\
	return 1;\
}\
if(fwrite(&elf_header(bits), sizeof(elf_header(bits)), 1, fd) != 1){\
	perror("Obfuscating header\n");\
	return 1;\
}

int main(int argc, char* argv[]){
	int i;
	char* section_name;
	if(argc != 2){
		printf("Usage: %s <target>\n", argv[0]);
		return 1;
	}
	if((fd = fopen(argv[1], "r+")) > 0){
		if(fread(&elf_header(32), sizeof(elf_header(32)), 1, fd) != 1){
			perror("Error parsing target");
			return 1;
		}
		switch(elf_header(32).e_machine){
			case 62:
				fseek(fd, 0, SEEK_SET);
				if(fread(&elf_header(64), sizeof(elf_header(64)), 1, fd) != 1){
					perror("Error parsing target");
					return 1;
				}
				troll(64);
				break;
			default:
				troll(32);
				break;
		}
		printf("Done\n");
		return 0;
	}
	perror("Opening target");
	return 1;
}

void __attribute__((destructor)) cleanup(void){
	if(strtab){
		free(strtab);
	}
	if(fd){
		fclose(fd);
	}
}

