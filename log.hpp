#pragma once

#include <usart.hpp>

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

class Log {

private:
    static Log* m_instance;
    static AsyncUSART m_usart;

    // make constructors private
    Log() {}
    Log(const Log&);
    ~Log() {}

    class CGuard {
        public:
            ~CGuard() {
                if (Log::m_instance != NULL) {
                    delete m_instance;
                    Log::m_instance = NULL;
                }
            }
    };

public:
    static Log* getInstance() {
        static CGuard g;
        if (!m_instance) {
            m_instance = new Log();
        }
        return m_instance;
    }

    void info(char* message) {
        m_usart.send("[INFO] ");
        m_usart.send(message);
        m_usart.send('\n');
    }

};

Log* Log::m_instance = 0;   // static class elements need to be initialized
