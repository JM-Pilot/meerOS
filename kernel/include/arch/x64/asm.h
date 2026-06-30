#include <stdint.h>
#ifndef _ARCH_X64_ASM_H
#define _ARCH_X64_ASM_H

static inline void port_outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t port_inb(uint16_t port)
{
	uint8_t ret;
	__asm__ volatile ( "inb %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}
static inline void port_outw(uint16_t port, uint16_t val)
{
	__asm__ volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint16_t port_inw(uint16_t port)
{
	uint16_t ret;
	__asm__ volatile ( "inw %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}

static inline void port_outl(uint16_t port, uint32_t val)
{
	__asm__ volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint32_t port_inl(uint16_t port)
{
	uint32_t ret;
	__asm__ volatile ( "inl %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}
#endif