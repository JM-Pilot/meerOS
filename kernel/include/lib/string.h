#include <stdint.h>
#include <stddef.h>
#ifndef _KERNEL_LIB_STRING_H
#define _KERNEL_LIB_STRING_H

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

#endif /* _KERNEL_LIG_STRING_H */