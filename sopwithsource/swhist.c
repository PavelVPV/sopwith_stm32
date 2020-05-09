/*

    swgrph   -  SW history processing   ( dummy )

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
            84-02-21    Development
            84-06-13    PCjr Speed-up
            85-11-05    Atari
            87-03-09    Microsoft compiler.
            2003-01-27  GNU General Public License
            2020-04-10  Convert code to ANSI C
*/

#include "sw.h"

unsigned int histinit(unsigned int seed)
{
    return swrand_hal_random_number_get();
}

void history(void)
{
    assert_param(FALSE);
}

/* Used for multiuser game */
int histmult( int index, int key )
{
    assert_param(FALSE);
    return 0;
}

void histend(void)
{
    assert_param(FALSE);
}
