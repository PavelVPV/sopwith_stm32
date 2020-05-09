/*
    swdispc  -  Display all players and objects

    Copyright (C) 1984-2003 David L. Clark.
    Copyright (c) 2020 Pavel Vasilyev.

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

#ifndef SWDISP_H
#define SWDISP_H

#include "sw.h"

void dispplyr( OBJECTS *ob );
void dispbomb( OBJECTS *obp );
void dispmiss( OBJECTS *obp );
void dispburst( OBJECTS *obp );
void dispexpl( OBJECTS *obp );
void dispcomp( OBJECTS *ob );
void dispmult( OBJECTS *ob );
void disptarg( OBJECTS *ob );
void dispflck( OBJECTS *ob );
void dispbird( OBJECTS *ob );
void dispwobj( OBJECTS *obp );
void dispwindshot(void);
void dispsplatbird(void);
void dispoxsplat(void);

#endif // SWDISP_H
