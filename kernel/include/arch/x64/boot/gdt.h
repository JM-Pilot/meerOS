#include <stdint.h>
#ifndef _ARCH_X64_BOOT
#define _ARCH_X64_BOOT

#define NULL_SELECTOR 0x00
#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS 0x18
#define USER_DS 0x20

struct gdt_desc {
	uint16_t size;
	uint64_t offset;
}__attribute__((packed));

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t flags;
	uint8_t base_high;
}__attribute__((packed));

void gdt_set_entry(uint8_t n,
	uint32_t limit, uint32_t base, 
	uint8_t access, uint8_t flags);
void gdt_init();
#endif