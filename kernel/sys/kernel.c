#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limine.h>
#include <psf.h>
#include <boot/requests.h>
#include <arch/x64/boot/gdt.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/console.h>
// Halt and catch fire function.
static void hcf(void)
{
	for (;;) {
		asm ("hlt");
	}
}

/* We check these requests first before anything, 
 * or else the kernel will not function properly
 */
static void check_requests(void)
{
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
		hcf();
	if (framebuffer_request.response == NULL 
		|| framebuffer_request.response->framebuffer_count < 1)
		hcf();
}

struct console_hdr kcon;
/* Entry to the kernel 
 * init all important functions
 */
void kernel_main(void)
{
	check_requests();
	framebuffer_init();
	console_init(&kcon, PSF_TER_U18N, 8, 0xFFFFFF, 0);
	console_puts(&kcon, "(kcon) Kernel Console Initialized \n");
	gdt_init();
	console_puts(&kcon, "(cpu x64) GDT Initialized \n");
	hcf();
}