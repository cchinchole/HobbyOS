#include  <drivers/vga.h>
#include  <arch/i686/io.h>
#include  <libc/string.h>
#include  <libc/stdio.h>
#include  <drivers/font.h>
#include <stdint.h>

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ			0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define VGA_DAC_MASK		0x3C6
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)





int get_offset(int row, int col) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }

int ATTR = WHITE_ON_BLACK;


void set_attribute(uint16_t a)
{
	ATTR = a;
}

int get_cursor_offset()
{
    pbout(VGA_CRTC_INDEX, 14);
    int offset = pbin(VGA_CRTC_DATA) << 8;
    pbout(VGA_CRTC_INDEX, 15);
    offset += pbin(VGA_CRTC_DATA);
    return offset*2;
}

void set_cursor_offset(int offset)
{
    offset /= 2;
    pbout(VGA_CRTC_INDEX, 14);
    pbout(VGA_CRTC_DATA, (unsigned char)(offset >> 8));
    pbout(VGA_CRTC_INDEX, 15);
    pbout(VGA_CRTC_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen()
{
    int size_screen = MAX_COLS * MAX_ROWS;
    unsigned char* screen = (unsigned char*)VIDEO_ADDRESS;

    for(int i = 0; i < size_screen; i++)
    {
        screen[i*2] = ' ';
        screen[i*2+1] = ATTR;
    }
    set_cursor_offset(get_offset(0, 0));

}

int put_char_at(char c, int row, int col, int color)
{
    unsigned char* screen = (unsigned char*)VIDEO_ADDRESS;
    int offset = 0;
    switch(c)
    {
        case '\n':
            offset = get_offset(row+1, 0);
            break;
        case '\b':
            offset = get_cursor_offset()-2;
            screen[offset] = (char)(0x00);
            screen[offset+1] = color;
            break;
        default:
            offset = get_offset(row, col);
            screen[offset] = c;
            screen[offset+1] = color;
            offset+=2;
            break;
    }

    if(offset >= MAX_ROWS*MAX_COLS*2)
    {
        for(int i = 1; i < MAX_ROWS; i++)
            memcpy((uint8_t*)(get_offset(i-1, 0) + VIDEO_ADDRESS), (uint8_t*)(get_offset(i, 0) + VIDEO_ADDRESS), MAX_COLS*2);

        char* line = (char*)(get_offset(MAX_ROWS-1, 0) + VIDEO_ADDRESS);
        for(int i = 0; i < MAX_COLS*2; i++)
            line[i] = 0;

        offset -= MAX_COLS*2;
    }

    set_cursor_offset(offset);
    return offset;
}

int put_char(char c)
{
    int row = get_offset_row(get_cursor_offset());
    int col = get_offset_col(get_cursor_offset());
    int colr = ATTR;
    return put_char_at(c, row, col, colr);  
}

void kprint_at(char* message, int row, int col) {
    int i = 0;
    int offset = 0;
    while (message[i] != 0) {
        offset = put_char_at(message[i], row, col, ATTR);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
        i++;
    }
}


typedef uint32_t  FARPTR;
#define MK_FP(seg, off)        ((FARPTR) (((uint32_t) (seg) << 16) | (uint16_t) (off)))
#define FP_SEG(fp)             (((FARPTR) fp) >> 16)
#define FP_OFF(fp)             (((FARPTR) fp) & 0xffff)
#define FP_TO_LINEAR(seg, off) ((void*) ((((uint16_t) (seg)) << 4) + ((uint16_t) (off))))





#define	peekb(S,O)		*(unsigned char *)(16uL * (S) + (O))
#define	pokeb(S,O,V)		*(unsigned char *)(16uL * (S) + (O)) = (V)
#define	pokew(S,O,V)		*(unsigned short *)(16uL * (S) + (O)) = (V)
#define	_vmemwr(DS,DO,S,N)	memcpy((char *)((DS) * 16 + (DO)), S, N)


static unsigned get_fb_seg(void)
{
	unsigned seg;

	pbout(VGA_GC_INDEX, 6);
	seg = pbin(VGA_GC_DATA);
	seg >>= 2;
	seg &= 3;
	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA000;
		break;
	case 2:
		seg = 0xB000;
		break;
	case 3:
		seg = 0xB800;
		break;
	}
	return seg;
}

static void vpokeb(unsigned off, unsigned val)
{
	pokeb(get_fb_seg(), off, val);
}
/*****************************************************************************
*****************************************************************************/
static unsigned vpeekb(unsigned off)
{
	return peekb(get_fb_seg(), off);
}

static void vmemwr(unsigned dst_off, unsigned char *src, unsigned count)
{
	_vmemwr(get_fb_seg(), dst_off, src, count);
}



static void (*g_write_pixel)(unsigned x, unsigned y, unsigned c);
static unsigned g_wd, g_ht;

static void write_pixel2(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes, off, mask;

	c = (c & 3) * 0x55;
	wd_in_bytes = g_wd / 4;
	off = wd_in_bytes * y + x / 4;
	x = (x & 3) * 2;
	mask = 0xC0 >> x;
	vpokeb(off, (vpeekb(off) & ~mask) | (c & mask));
}

static void write_pixel8(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes;
	unsigned off;

	wd_in_bytes = g_wd;
	off = wd_in_bytes * y + x;
	vpokeb(off, c);
}

static void set_plane(unsigned p)
{
	unsigned char pmask;

	p &= 3;
	pmask = 1 << p;
/* set read plane */
	pbout(VGA_GC_INDEX, 4);
	pbout(VGA_GC_DATA, p);
/* set write plane */
	pbout(VGA_SEQ_INDEX, 2);
	pbout(VGA_SEQ_DATA, pmask);
}


static void write_pixel4p(unsigned x, unsigned y, unsigned c)
{
	unsigned wd_in_bytes, off, mask, p, pmask;

	wd_in_bytes = g_wd / 8;
	off = wd_in_bytes * y + x / 8;
	x = (x & 7) * 1;
	mask = 0x80 >> x;
	pmask = 1;
	for(p = 0; p < 4; p++)
	{
		set_plane(p);
		if(pmask & c)
			vpokeb(off, vpeekb(off) | mask);
		else
			vpokeb(off, vpeekb(off) & ~mask);
		pmask <<= 1;
	}
}


static void draw_x(void)
{
	unsigned x, y;

/* clear screen */
	for(y = 0; y < g_ht; y++)
		for(x = 0; x < g_wd; x++)
			g_write_pixel(x, y, 0);
/* draw 2-color X */

	for(int i = 0; i < 40; i++)
		for(int x = 0; x < 40; x++)
		{
			g_write_pixel(25+i, 25+x, 255);
		}

		
}



void write_regs(unsigned char *regs)
{
	unsigned i;

/* write MISCELLANEOUS reg */
	pbout(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		pbout(VGA_SEQ_INDEX, i);
		pbout(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	pbout(VGA_CRTC_INDEX, 0x03);
	pbout(VGA_CRTC_DATA, pbin(VGA_CRTC_DATA) | 0x80);
	pbout(VGA_CRTC_INDEX, 0x11);
	pbout(VGA_CRTC_DATA, pbin(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		pbout(VGA_CRTC_INDEX, i);
		pbout(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		pbout(VGA_GC_INDEX, i);
		pbout(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)pbin(VGA_INSTAT_READ);
		pbout(VGA_AC_INDEX, i);
		pbout(VGA_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	(void)pbin(VGA_INSTAT_READ);
	pbout(VGA_AC_INDEX, 0x20);
}


static void write_font(unsigned char *buf, unsigned font_height)
{
	unsigned char seq2, seq4, gc4, gc5, gc6;
	unsigned i;

/* save registers
set_plane() modifies GC 4 and SEQ 2, so save them as well */
	pbout(VGA_SEQ_INDEX, 2);
	seq2 = pbin(VGA_SEQ_DATA);

	pbout(VGA_SEQ_INDEX, 4);
	seq4 = pbin(VGA_SEQ_DATA);
/* turn off even-odd addressing (set flat addressing)
assume: chain-4 addressing already off */
	pbout(VGA_SEQ_DATA, seq4 | 0x04);

	pbout(VGA_GC_INDEX, 4);
	gc4 = pbin(VGA_GC_DATA);

	pbout(VGA_GC_INDEX, 5);
	gc5 = pbin(VGA_GC_DATA);
/* turn off even-odd addressing */
	pbout(VGA_GC_DATA, gc5 & ~0x10);

	pbout(VGA_GC_INDEX, 6);
	gc6 = pbin(VGA_GC_DATA);
/* turn off even-odd addressing */
	pbout(VGA_GC_DATA, gc6 & ~0x02);
/* write font to plane P4 */
	set_plane(2);
/* write font 0 */
	for(i = 0; i < 256; i++)
	{
		vmemwr(16384u * 0 + i * 32, buf, font_height);
		buf += font_height;
	}

/* restore registers */
	pbout(VGA_SEQ_INDEX, 2);
	pbout(VGA_SEQ_DATA, seq2);
	pbout(VGA_SEQ_INDEX, 4);
	pbout(VGA_SEQ_DATA, seq4);
	pbout(VGA_GC_INDEX, 4);
	pbout(VGA_GC_DATA, gc4);
	pbout(VGA_GC_INDEX, 5);
	pbout(VGA_GC_DATA, gc5);
	pbout(VGA_GC_INDEX, 6);
	pbout(VGA_GC_DATA, gc6);
}


void set_text_mode(int hi_res)
{
	unsigned rows, cols, ht, i;

	if(hi_res)
	{
		write_regs(g_90x60_text);
		cols = 90;
		rows = 60;
		ht = 8;
	}
	else
	{
		write_regs(g_80x25_text);
		cols = 80;
		rows = 25;
		ht = 16;
	}
/* set font */
	if(ht >= 16)
		write_font(g_8x16_font, 16);
	else
		write_font(g_8x8_font, 8);
/* tell the BIOS what we've done, so BIOS text output works OK */
	pokew(0x40, 0x4A, cols);	/* columns on screen */
	pokew(0x40, 0x4C, cols * rows * 2); /* framebuffer size */
	pokew(0x40, 0x50, 0);		/* cursor pos'n */
	pokeb(0x40, 0x60, ht - 1);	/* cursor shape */
	pokeb(0x40, 0x61, ht - 2);
	pokeb(0x40, 0x84, rows - 1);	/* rows on screen - 1 */
	pokeb(0x40, 0x85, ht);		/* char height */
/* set white-on-black attributes for all text */
	for(i = 0; i < cols * rows; i++)
		pokeb(0xB800, i * 2 + 1, 7);
}

void init_vga()
{
	set_text_mode(0);
    clear_screen();
	klog(LOG_INFO, "Vga initalizing.", 1);
	klog(LOG_SUCCESS, "Vga initalized.", 1);
}

void demo_graphics()
{
    /* 256-color */
	write_regs(g_640x480x16);
	g_wd = 640;
	g_ht = 480;
	g_write_pixel = write_pixel4p;
	draw_x();
}