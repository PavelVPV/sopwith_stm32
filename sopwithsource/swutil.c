/*
    swutil   -  SW assembler utilities

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
            84-02-23    Development
            84-06-12    PCjr Speed-up
            84-06-15    Joystick support
            87-03-10    Microsoft compiler
            87-03-04    Missile and starburst support
            87-03-05    Remap missile and starburst keys
            87-04-09    Fix to non-IBM keyboard.
            94-12-18    C6 Compiler
            2003-01-27  GNU General Public License
            2020-04-10  Convert code to ANSI C
 */

#include "sw.h"

#include <string.h>

void setdxdy(OBJECTS *obj, int dx, int dy)
{
    // See swuitl.asm
    obj->ob_dx = dx >> 8;
    obj->ob_ldx = (dx & 0xFF) << 8;

    obj->ob_dy = dy >> 8;
    obj->ob_ldy = (dy & 0xFF) << 8;
}

void intson(void)
{
    // Nothing to do
}

void intsoff(void)
{
    // Nothing to do
}

int sounddiv(int a, int b)
{
    assert_param(FALSE);
    return 0;
}

int soundmul(int a, int b, int c)
{
    assert_param(FALSE);
    return 0;
}

void movexy(OBJECTS *ob, int *x, int *y)
{
    // See swuitl.asm
    uint32_t val;

    val = (ob->ob_x + ob->ob_dx) << 16;

    ob->ob_x = (uint16_t) (val >> 16);
    ob->ob_lx = (uint16_t) val;

    *x = ob->ob_x;

    val = (ob->ob_y + ob->ob_dy) << 16;

    ob->ob_y = (uint16_t) (val >> 16);
    ob->ob_ly = (uint16_t) val;

    *y = ob->ob_y;
}

static BOOL timer_started = FALSE;

void swtick(void)
{
    if (timer_started == TRUE)
    {
        swtickc();
    }
}

void swtimerstart(void)
{
    timer_started = TRUE;
}

void movmem(void *src,void *dest,unsigned count)
{
    memmove(dest,src,count);
}
