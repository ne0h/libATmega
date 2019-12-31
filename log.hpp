#pragma once

#include <stdio.h>

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

/**
 * Log is a very simple logging framework which leverages USART0.
 * Log is implemented as singelton.
 */
class Log {

private:
    static Log* m_instance;
    static AsyncUSART m_usart;

    // make constructors private
    Log() {}
    Log(const Log&);
    ~Log() {}

    void vlog(size_t bufSize, const char* format, va_list args) {
        char buf[bufSize];
        vsprintf(buf, format, args);

        m_usart.send(buf);
        m_usart.send('\n');
    }

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
    /**
     * Returns a pointer to the instance of Log.
     * \return a pointer to the instance of Log.
     */
    static Log* getInstance() {
        static CGuard g;
        if (!m_instance) {
            m_instance = new Log();
        }
        return m_instance;
    }

    /**
     * Adds a new line to the log. Makes use of sprintf() to format the result.
     * Uses a pre-defined length of 128 to format the result.
     * \param format The format for the result.
     */
    void log(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vlog(128, format, args);
        va_end(args);
    }

    /**
     * Adds a new line to the log. Makes use of sprintf() to format the result.
     * \param bufSize The size of the buffer used to format the result.
     * \param format The format for the result.
     */
    void log(size_t bufSize, const char* format, ...) {
        va_list args;
        va_start(args, format);
        vlog(bufSize, format, args);
        va_end(args);
    }

};

Log* Log::m_instance = 0;   // static class elements need to be initialized
