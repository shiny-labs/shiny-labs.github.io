//Compile with gcc cryptme.c -g -fomit-frame-pointer -Wl,-e__decryption_stub

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define CRYPT_SECTION ".elf"

void section_start() __attribute__((section(CRYPT_SECTION)))
    __attribute__((aligned(4096))); //So memprotect works on page aligned data
int main() __attribute__((section(CRYPT_SECTION)));
void sentinal() __attribute__((section(CRYPT_SECTION)));

void section_start()
{                                                     //Mark start
    __asm__ volatile("nop\n\tnop\n\tnop\n\tnop\n\t"); // & start + 4 = \x90\x90\x90\x90
}

int main() // Encrypted main function
{
    char buf[16];
    buf[0] = 'I';
    buf[1] = 'n';
    buf[2] = ' ';
    buf[3] = 'c';
    buf[4] = 'r';
    buf[5] = 'y';
    buf[6] = 'p';
    buf[7] = 't';
    buf[8] = 'e';
    buf[9] = 'd';
    buf[10] = ' ';
    buf[11] = 'f';
    buf[12] = 'u';
    buf[13] = 'n';
    buf[14] = 'c';
    buf[15] = '!';
    printf("%s\n", buf);
    exit(0);
}

void sentinal() {} //Mark end

int __decryption_stub()
{ //This is our entrypoint
    unsigned char *ptr = (unsigned char *)&section_start;

    unsigned int len = (&sentinal) - (&section_start);

    //Call mprotect() so we can modify ourselves.

    mprotect(&section_start, len, PROT_READ|PROT_WRITE|PROT_EXEC);

    printf("Bruteforcing the decryption key...\n");
    unsigned int key = 0;
    for (unsigned int i = 0; i < 0xFFFFFFFF; i++)
    {
        if (((*(ptr) ^ i) == 0x90) && ((*(ptr + 1) ^ i) == 0x90) &&
            ((*(ptr + 2) ^ i) == 0x90) && ((*(ptr + 3) ^ i) == 0x90))
        { //Bruteforce keyspace
            key = i;
            break;
        }
    }
    printf("Found the key! %d\n", key);
    for (int i = 0; i < len; i++)
    {
        unsigned char v = *(ptr + i);
        *(ptr + i) = v ^ key; //Do decryption
    }
    main();
}
