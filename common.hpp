#pragma once

enum Result {
	Success,
	TimeoutError,
	ChecksumError,
};

// source: https://www.avrfreaks.net/forum/avr-c-micro-how?name=PNphpBB2&file=viewtopic&t=59453

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};

void* operator new(size_t size) {
    return malloc(size);
}

void operator delete(void* ptr, unsigned int i) {
    free(ptr);
}
