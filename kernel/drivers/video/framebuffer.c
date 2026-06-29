#include <stdint.h>
#include <limine.h>
#include <string.h>
#include <boot/requests.h>
struct limine_framebuffer *framebuffer_main;
uint32_t *video_ptr;
void framebuffer_init()
{
	framebuffer_main = framebuffer_request.response->framebuffers[0];
	video_ptr = (uint32_t*)framebuffer_main->address;
}
void framebuffer_put_pix(uint32_t c, int x, int y)
{
	video_ptr[y * (framebuffer_main->pitch / 4) + x] = c;
}
uint32_t framebuffer_get_pix(int x, int y) 
{
	return video_ptr[y * (framebuffer_main->pitch / 4) + x];
}
void framebuffer_clear(uint32_t c)
{
	for (uint64_t y = 0; y < framebuffer_main->height; y++)
		for (uint64_t x = 0; x < framebuffer_main->width; x++)
			framebuffer_put_pix(c, x, y);
}
