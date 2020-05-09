/*
	swmiscjr -	SW miscellaneous

	Copyright (C) 1984-2003 David L. Clark.
	Copyright (C) 2020 Pavel Vasilyev.

	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version. This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details. You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free Software Foundation,
	Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 

			Author: Dave Clark
			        Pavel Vasilyev

	Modification History:
			84-07-23	Development
			87-03-09	Microsoft compiler.
			2003-01-27	GNU General Public License
			2020-04-10  Convert code to ANSI C
*/

#include	"sw.h"
#include    <string.h>

char    videoram[SCR_WDTH * SCR_HGHT];      /* "Main" display area. Used for collisions. */
char    auxdisp[0];                         /* Auxiliary display area. Kept for history. */
char    *dispoff;                           /* Current display offset       */

static uint16_t m_x;
static uint16_t m_y;

void swposcur(int x, int y)
{
    assert_param(x >= 0);
    assert_param(y >= 0);

    m_x = (uint16_t) x;
    m_y = (uint16_t) y;
}

void swcolour(int x)
{
    assert_param(x >= 0 && x < 0xFFFF);
    swdisplay_hal_set_color(x);
}

void swputs(char *sp)
{
    if (!strcmp(sp, "\r\n"))
    {
        m_x = 0;
        m_y++;
    }
    else
    {
        swdisplay_hal_put_string(m_x, m_y, (uint8_t *) sp);
    }
}

int swputc(char ch)
{
    swdisplay_hal_put_char(m_x, m_y, (uint8_t) ch);
    m_x++;
    return ch;
}

void swflush(void)
{
    /* Related to keyboard. Nothing to do. */
}

void swscrbufclr(void)
{
    memset(videoram, 0, sizeof(videoram));

    swdisplay_hal_screen_clear();
}

void swscrclr(void)
{
    swdisplay_hal_screen_clear();
}


void swpxlxor(int offset, char color)
{
    if (!dispoff)
    {
        swdisplay_hal_pixel_xor(offset, color);
    }
    else
    {
        dispoff[offset] ^= color;
    }
}

void swpxland(int offset, char color)
{
    if (!dispoff)
    {
        swdisplay_hal_pixel_and(offset, color);
    }
    else
    {
        dispoff[offset] &= color;
    }
}

void swpxlor(int offset, char color)
{
    if (!dispoff)
    {
        swdisplay_hal_pixel_or(offset, color);
    }
    else
    {
        dispoff[offset] |= color;
    }
}

char swpxlget(int offset)
{
    if (!dispoff)
    {
        return swdisplay_hal_pixel_get(offset);
    }
    else
    {
        return dispoff[offset];
    }
}
