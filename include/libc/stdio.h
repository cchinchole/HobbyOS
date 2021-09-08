#ifndef _STDIO_H
#define _STDIO_H
#define EOF (-1)
#define LOG_SUCCESS 0
#define LOG_ERROR 1
#define LOG_INFO 2

int kprintf(const char* restrict format, ...);
void klog(int logType, char* message, int endLine);
#endif