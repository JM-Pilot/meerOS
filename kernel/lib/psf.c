#include <psf.h>
#include <stdint.h>
#include <drivers/video/framebuffer.h>

/* returns 1 if its psf1
 * returns 0 if its psf2
 * returns 2 if not
 */
int psf_check_data(char *font_data)
{
	uint8_t *data = (uint8_t*)font_data;
	if (data[0] == 0x36 && data[1] == 0x04)
		return 1;
	else if (data[0] == 0x72 && data[1] == 0xB5 &&
		 data[2] == 0x4A && data[3] == 0x86)
		return 0;
	return 2;
}
/* initialize a psf1 font 
 * return 1 if its not a psf1 font
 * return 0 if successful 
 */
int psf1_init_font(struct psf1_font_hdr **font, char *font_data)
{
	*font = (struct psf1_font_hdr*)font_data;
	if ((*font)->magic != PSF1_MAGIC)
		return 1;
	(*font)->font_data = font_data;
	return 0;
}

/* draws a psf1 font */
void psf1_draw_char(struct psf1_font_hdr *font, uint8_t c,
		    int x, int y, uint32_t fg, uint32_t bg)
{
	uint8_t *glyphs = (uint8_t *)font->font_data;
	uint8_t *glyph = glyphs + c * (font)->char_size;

	for (uint8_t gy = 0; gy < font->char_size; gy++)
	{
		uint8_t byte = glyph[gy];
		for (uint8_t gx = 0; gx < 8; gx++) {
			if (byte & (0x80 >> gx))
				framebuffer_put_pix(fg, x + gx, y + gy);
			else
				framebuffer_put_pix(bg, x + gx, y + gy);
		}
	}
}

/* initialize a psf2 font 
 * return 1 if its not a psf2 font
 * return 0 if successful 
 */
int psf2_init_font(struct psf2_font_hdr **font, char *font_data)
{
	*font = (struct psf2_font_hdr*)font_data;
	if ((*font)->magic != PSF2_MAGIC)
		return 1;
	(*font)->font_data = font_data;
	return 0;
}

/* draws a psf2 font */
void psf2_draw_char(struct psf2_font_hdr *font, uint8_t c, 
		    int x, int y, uint32_t fg, uint32_t bg)
{
	uint8_t *glyph = (uint8_t*)font->font_data +
		font->header_sz + c * font->bpg;

	uint32_t bpgln = (font->width + 7) / 8;
	for (uint32_t gy = 0; gy < font->height; gy++) {
		uint8_t *cbyte = glyph + (bpgln * gy);
		uint8_t mask = 1 << 7;
		for (uint32_t gx = 0; gx < font->width; gx++) {
			if (*cbyte & mask)
				framebuffer_put_pix(fg, x + gx, y + gy);
			else
				framebuffer_put_pix(bg, x + gx, y + gy);
			mask >>= 1;
			if (mask == 0) {
				mask = 1 << 7;
				cbyte += 1;
			}
		}
	}
}

