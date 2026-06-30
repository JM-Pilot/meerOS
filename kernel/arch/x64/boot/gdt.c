#include <stdint.h>
#include <arch/x64/boot/gdt.h>

/* this will load the gdt */
extern void gdt_load(void);

/* kernel(2) + user(2) + null = 5 */
static struct gdt_entry gdt[5];
struct gdt_desc gdtr;

void gdt_set_entry(uint8_t n,
	uint32_t limit, uint32_t base, 
	uint8_t access, uint8_t flags)
{	
	gdt[n].base_low = base & 0xFFFF;
	gdt[n].base_mid = (base >> 16) & 0xFF;
	gdt[n].base_high = (base >> 24) & 0xFF;

	gdt[n].access = access;
	gdt[n].limit_low = limit & 0xFFFF;
	/* we will need flags + limit */
	/* example flags = 0xC limit = 0xF, output = 0xCF */
	gdt[n].flags = (flags & 0xF) << 4 | (limit & 0xF);
}

/* Sets up the x64/amd64 GDT and loads it */
void gdt_init(void)
{

	gdtr.offset = (uint64_t)&gdt;
	gdtr.size = sizeof(gdt) - 1;

	/* NULL */
	gdt_set_entry(0, 0, 0, 0, 0);

	/* KERNEL */
	gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xA);
	gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xC);

	/* USER */
	gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xA);
	gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xC);

	gdt_load();
}