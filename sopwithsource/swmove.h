/*
    swmove   -  SW move all objects and players

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

#ifndef SWMOVE_H
#define SWMOVE_H

void swmove(void);
BOOL moveplyr( OBJECTS *obp );
void interpret( OBJECTS *obp, int key );
BOOL movecomp( OBJECTS *obp );
BOOL movepln( OBJECTS *obp );
BOOL moveshot( OBJECTS *obp );
BOOL movebomb( OBJECTS *obp );
BOOL movemiss( OBJECTS *obp );
BOOL moveburst( OBJECTS *obp );
BOOL movetarg(OBJECTS *obt);
BOOL moveexpl( OBJECTS *obp );
BOOL movesmok( OBJECTS *obp );
BOOL moveflck( OBJECTS *obp );
BOOL movebird( OBJECTS *obp );
BOOL moveox( OBJECTS *ob );
void crashpln( OBJECTS *obp );
void hitpln( OBJECTS *obp );
void stallpln( OBJECTS *obp );
void insertx(OBJECTS *ob, OBJECTS *obp );
void deletex(OBJECTS *obp );

#endif // SWMOVE_H
