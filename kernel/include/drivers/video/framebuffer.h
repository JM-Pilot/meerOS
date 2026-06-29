#include <stdint.h>
#include <limine.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

extern struct limine_framebuffer *framebuffer_main;

void framebuffer_init();
void framebuffer_put_pix(uint32_t c, int x, int y);
void framebuffer_clear(uint32_t c);
uint32_t framebuffer_get_pix(int x, int y);
#endif