/*
    swinit   -  SW initialization

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

#ifndef SWINIT_H
#define SWINIT_H

#include "sw.h"

void swinit(int argc, char *argv[]);
void initseed(void);
void swrestart(void);
void initgdep(void);
void clrprmpt(void);
int getgame(void);
BOOL ctlbreak(void);
void initdisp(BOOL reset);
void dispcgge(OBJECTS *ob);
void dispfgge(OBJECTS *ob);
void dispbgge(OBJECTS *ob);
void dispsgge(OBJECTS *ob);
void dispmgge(OBJECTS *ob);
void dispsbgge(OBJECTS *ob);
void initcomp(OBJECTS *obp);
void initplyr(OBJECTS *obp);
OBJECTS *initpln(OBJECTS *obp);
void initshot(OBJECTS *obop, OBJECTS *targ);
void initbomb(OBJECTS *obop);
void initmiss(OBJECTS *obop);
void initburst(OBJECTS *obop);
void initexpl(OBJECTS *obop, int	small);
void initsmok(OBJECTS *obop);
void initflck(void);
void initbird(OBJECTS *obop, int	i);
void initoxen(void);

#endif // SWINIT_H
