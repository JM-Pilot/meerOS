#include <stdarg.h>
#ifndef _KERNEL_LIB_PRINTK_H
#define _KERNEL_LIB_PRINTK_H

/* all of these printf functions outputs to kernel console (kcon) */

int kvsprintf(char *buf, const char *fmt, va_list args);
int kprintf(const char *fmt, ...);

#endif /* _KERNEL_LIB_PRINTK_H */