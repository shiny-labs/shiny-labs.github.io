#ifndef _STUB_H
#define _STUB_H
#include <sys/mman.h>
extern void* __libc_csu_init;
static void dummy();
#define round_down(to_round,page_size) ((((unsigned long) to_round)/page_size)*page_size)
#define round_up(to_round,page_size) (round_down(to_round, page_size) + page_size)
static void __attribute__((constructor)) stub(void){
	void* start = &dummy;
	void* end = &__libc_csu_init;
	size_t code_size = (((unsigned long) end)) - ((unsigned long) start);
	size_t page_size = getpagesize();
	mprotect((void*) round_down(start, page_size), round_up(code_size, page_size), PROT_READ | PROT_WRITE | PROT_EXEC);
	memfrob(&dummy, code_size);
	mprotect((void*) round_down(start, page_size), round_up(code_size, page_size), PROT_READ | PROT_EXEC);
}
static void dummy(){}
#endif

