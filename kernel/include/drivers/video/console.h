#include <stdint.h>
#include <stdbool.h>
#include <psf.h>

#ifndef _DRIVERS_VIDEO_CONSOLE_H
#define _DRIVERS_VIDEO_CONSOLE_H

struct console_hdr {
	int cx, cy;
	uint32_t fg, bg;
	int cellsx, cellsy;
	int tabw;
	bool is_psf1;
	struct psf1_font_hdr *psf1_font;
	struct psf2_font_hdr *psf2_font;
};	

/* Params:
 * **console - address of console
 * is_psf1 - true to use psf1 and false to use psf2
 * *font_data - font data to use load the font
 * fg, bg - colors
 */
void console_init(struct console_hdr *console, 
		  char *font_data, int tabw,
		  uint32_t fg, uint32_t bg);
void console_putc(struct console_hdr *console, char c);
void console_puts(struct console_hdr *console, const char *s);
void console_set_col(struct console_hdr *console, uint32_t fg, uint32_t bg);
void console_putnl(struct console_hdr *console);
#endif /* _DRIVERS_VIDEO_CONSOLE_H */