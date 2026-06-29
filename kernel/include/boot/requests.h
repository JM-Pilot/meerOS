#include <stdint.h>
#include <limine.h>
#ifndef _BOOT_REQUESTS_H
#define _BOOT_REQUESTS_H

extern volatile uint64_t limine_base_revision[];
extern volatile struct limine_framebuffer_request framebuffer_request;

#endif