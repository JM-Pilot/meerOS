#include <stdint.h>
#include <printk.h>
#include <arch/x64/int/idt.h>
#include <arch/x64/boot/gdt.h>
#define MAX_ENTRIES 256

extern uint64_t isr_stub_table[32];
static struct idt_entry idt[MAX_ENTRIES];
static struct idt_desc idtr;
void idt_set_entry(uint8_t n, uint64_t offset,
		   uint16_t selector, uint8_t flags)
{
	idt[n].offset_low = offset & 0xFFFF;
	idt[n].offset_mid = (offset >> 16) & 0xFFFF;
	idt[n].offset_high = (offset >> 32) & 0xFFFFFFFF;
	idt[n].selector = selector;
	idt[n].flags = flags;
	idt[n].ist = 0;
	idt[n].rsvd = 0;
}
void idt_init(void)
{
	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	for (int i = 0; i < MAX_ENTRIES; i++)
		idt_set_entry(i, 0, 0, 0);
	for (int i = 0; i < 32; i++)
		idt_set_entry(i, isr_stub_table[i], KERNEL_CS, 0x8E);
	__asm__ volatile("lidt %0" :: "m"(idtr));
}

static const char *exceptions[] = {
	"DIVISION ERROR",
	"DEBUG",
	"NON MASKABLE INTERRUPT",
	"BREAKPOINT",
	"OVERFLOW",
	"BOUND RANGE EXCEEDED",
	"INVALID OPCODE",
	"DEVICE NOT AVAILABLE",
	"DOUBLE FAULT",
	"CPU RESERVED",
	"INVALID TSS",
	"SEGMENT NOT PRESENT",
	"STACK-SEGMENT FAULT",
	"GENERAL PROTECTION FAULT",
	"PAGE FAULT",
	"CPU RESERVED",
	"x87 FPU EXCEPTION",
	"ALIGNMENT CHECK",
	"MACHINE CHECK"
	"SIMD FPU EXCEPTION",
	"VIRTUALIZATION EXCEPTION",
	"CONTROL PROTECTION EXCEPTION",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"CPU RESERVED",
	"HYPERVISOR INJECTION EXCEPTION",
	"VMM COMMUNICATION EXCEPTION",
	"SECURITY EXCEPTION",
	"CPU RESERVED"
};
void int_exception_handler(struct interrupt_frame *intf) {
	if (intf->int_num < 32) {
		kprintf("ERROR, INTERRUPT REACHED\n");
		kprintf("INTERRUPT %s (%#.2X)\n", exceptions[intf->int_num], intf->int_num);

		kprintf("RAX: %#X RBX: %#X RCX: %#X RDX: %#X RSI: %#X\n",
			intf->rax, intf->rbx, intf->rcx, intf->rdx, intf->rsi);
		kprintf("RDI: %#X RBP: %#X R8: %#X  R9: %#X  R10: %#X\n",
			intf->rdi, intf->rbp, intf->r8, intf->r9, intf->r10);
		kprintf("R11: %#X R12: %#X R13: %#X R14: %#X R15: %#X\n",
			intf->r11, intf->r12, intf->r13, intf->r14, intf->r15);
		kprintf("RIP: %#X CS: %#.2X, RFLAGS: %#X RSP: %#X, SS: %#.2X\n",
			intf->rip, intf->cs, intf->rflags, intf->rsp, intf->ss);
		__asm__ volatile ("cli");
		for (;;) __asm__ volatile ("hlt");
	}
}