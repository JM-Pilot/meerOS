#include <stdint.h>
#include <stdbool.h>
#include <psf.h>
#include <string.h>
#include <drivers/video/framebuffer.h>
#include <drivers/video/console.h>

/* init the console */

void console_init(struct console_hdr *console, 
		  char *font_data, int tabw,
		  uint32_t fg, uint32_t bg)
{
	console->cx = 0;
	console->cy = 0;
	int font = psf_check_data(font_data);
	if (font == 1) {
		psf1_init_font(&console->psf1_font, font_data);
		console->is_psf1 = true;
		console->cellsx = framebuffer_main->width / 8;
		console->cellsy = framebuffer_main->height / console->psf1_font->char_size;
	} else if (font == 0) {
		psf2_init_font(&console->psf2_font, font_data);
		console->is_psf1 = false;
		console->cellsx = framebuffer_main->width / console->psf2_font->width;
		console->cellsy = framebuffer_main->height / console->psf2_font->height;
	}
	console->fg = fg;
	console->bg = bg;
	console->tabw = tabw;
}

/* scrolls the screen up */
void console_scroll_up(struct console_hdr *console)
{
	uint8_t *addr = (uint8_t*)framebuffer_main->address;
	uint32_t line = framebuffer_main->pitch * 
		console->is_psf1 == true ? 8 : console->psf2_font->height;

	memmove(addr, addr + line, framebuffer_main->pitch * framebuffer_main->height - line);
	memset(addr + framebuffer_main->pitch * framebuffer_main->height - line, 0, line);
	console->cy--;
}

/* puts newline */
void console_putnl(struct console_hdr *console)
{
	console->cx = 0;
	if (++console->cy >= console->cellsy)
		console_scroll_up(console);
}

/* check and print chars */

void console_putc(struct console_hdr *console, char c)
{
	struct psf2_font_hdr *con_psf2 = console->psf2_font;
	struct psf1_font_hdr *con_psf1 = console->psf1_font;
	switch (c) {
		case '\n':
			console_putnl(console);
			return;
		case '\r':
			console->cx = 0;
			return;
		case '\b':
			if (console->is_psf1 == true) {
				psf1_draw_char(con_psf1, ' ',
					       console->cx * 8, console->cy * con_psf1->char_size,
					       console->fg, console->bg);
			}
			else {
				psf2_draw_char(con_psf2, ' ',
					       console->cx * con_psf2->width, 
					       console->cy * con_psf2->height,
					       console->fg, console->bg);
			}
			if (console->cx > 0)
				console->cx--;
			return;
		case '\t':
			console->cx = (console->cx + console->tabw) & ~(console->tabw - 1);
			if (console->cx >= console->cellsx)
				console_putnl(console);
			return;
		default:
			if (console->is_psf1 == true)
				psf1_draw_char(con_psf1, c,
					       console->cx * 8, console->cy * con_psf1->char_size,
					       console->fg, console->bg);
			else
				psf2_draw_char(con_psf2, c,
					       console->cx * con_psf2->width, 
					       console->cy * con_psf2->height,
					       console->fg, console->bg);
			console->cx++;
			if (console->cx >= console->cellsx)
					console_putnl(console);
			return;
	}
	return;
}

/* prints a string */
void console_puts(struct console_hdr *console, const char *s)
{
	while (*s)
		console_putc(console, *s++);
}

/* switches the color in a console, does not change the screen immediately
 */
void console_set_col(struct console_hdr *console, uint32_t fg, uint32_t bg)
{
	uint32_t old_fg = console->fg;
	uint32_t old_bg = console->bg;
	console->fg = fg;
	console->bg = bg;

	for (uint32_t y = 0; y < framebuffer_main->height; y++) {
		for (uint32_t x = 0; x < framebuffer_main->width; x++) {
			uint32_t pix = framebuffer_get_pix(x, y);
			if (pix == old_fg)
				framebuffer_put_pix(fg, x, y);
			if (pix == old_bg)
				framebuffer_put_pix(bg, x, y);
		}
	}
}

