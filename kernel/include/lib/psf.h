#include <stdint.h>
#ifndef _KERNEL_LIB_PSF2_H
#define _KERNEL_LIB_PSF2_H

extern char _binary_misc_fonts_ter_u18n_psf_start[];
extern char _binary_misc_fonts_default8x16_psf_start[];
#define PSF_TER_U18N _binary_misc_fonts_ter_u18n_psf_start
#define PSF_DEF_8x16 _binary_misc_fonts_default8x16_psf_start
#define PSF1_MAGIC 0x0436

#define PSF1_MODE512 0x1
#define PSF1_MODEHASTAB 0x2
#define PSF1_MODESEQ 0x4


struct psf1_font_hdr {
	uint16_t magic;
	uint8_t mode;
	uint8_t char_size;
	char *font_data;
};

#define PSF2_MAGIC 0x864AB572
#define PSF2_HAS_UNICODE_TABLE 0x1

struct psf2_font_hdr{
	uint32_t magic;
	uint32_t version;
	uint32_t header_sz;
	uint32_t flags;
	uint32_t glyph_cnt;
	uint32_t bpg;	
	uint32_t height;
	uint32_t width;
	char *font_data;
};

int psf_check_data(char *font_data);

int psf1_init_font(struct psf1_font_hdr **font, char *font_data);
void psf1_draw_char(struct psf1_font_hdr *font, uint8_t c, 
		    int x, int y, uint32_t fg, uint32_t bg);

int psf2_init_font(struct psf2_font_hdr **font, char *font_data);
void psf2_draw_char(struct psf2_font_hdr *font, uint8_t c,
		    int x, int y, uint32_t fg, uint32_t bg);
#endif