/*

    swgrph   -  SW screen graphics

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
*/

#ifndef SWGRPHA_H
#define SWGRPHA_H

#include "sw.h"

void swdisp(void);
void swground(void);
void swclrcol(void);

void swputsym( int x, int y, OBJECTS *ob );
int swputcol( int x, int y, OBJECTS *ob );

void swpntsym( int x, int y, int clr );
int swpntcol( int x, int y, int clr );

void drawpc( int x, int y, int clr, int *oldclr );
void drawpm( int x, int y, int clr, int *oldclr );

int get_type(void);
void set_type( int type );

void setadisp(void);
void setvdisp(void);

#endif // SWGRPHA_H
