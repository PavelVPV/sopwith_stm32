/*
	swdispc  -	Display all players and objects

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

	Modification History:
			84-02-21	Development
			84-06-12	PCjr Speed-up
			87-03-09	Microsoft compiler.
			87-03-12	Wounded airplanes.
			87-03-13	Splatted bird symbol.
			87-04-05	Missile and starburst support
			2003-01-27	GNU General Public License
			2020-04-10  Convert code to ANSI C
*/

#include	"sw.h"

extern	OLDWDISP wdisp[];		/*  World display status	    */
extern	OBJECTS *nobjects;		/*  Objects list.		    */
extern	OBJECTS *objtop;		/*  Start of object list.	    */
extern	int	shothole;		/*  Number of shot holes to display */
extern	int	splatbird;		/*  Number of splatted birds	    */
extern	int	splatox;		/* Display splatted ox		    */
extern	int	oxsplatted;		/* An ox has been splatted	    */
extern	char	swshtsym[];		/*  Shot hole symbol		    */
extern	char	swsplsym[];		/*  Splatted bird symbol	    */
extern	int	countmove;		/*  Move number 		    */

static unsigned long seed = 74917777;

static void plnsound( OBJECTS *obp );
static void randsd(void);

void dispplyr( OBJECTS *ob )
{
	if ( shothole )
		dispwindshot();
	if ( splatbird )
		dispsplatbird();
	plnsound( ob );
}

void dispbomb( OBJECTS *obp )
{
    OBJECTS *ob;

	if ( ( ob = obp )->ob_dy <= 0 )
		sound( S_BOMB, -( ob->ob_y ), ob );
}

void dispmiss( OBJECTS *obp )
{
}

void dispburst( OBJECTS *obp )
{
}

void dispexpl( OBJECTS *obp )
{
    OBJECTS *ob;

	if ( ( ob = obp )->ob_orient )
		sound( S_EXPLOSION, ob->ob_hitcount, ob );
}

void dispcomp( OBJECTS *ob )
{
	plnsound( ob );
}

void dispmult( OBJECTS *ob )
{
	plnsound( ob );
}

void disptarg( OBJECTS *ob )
{
	if ( ob->ob_firing )
		sound( S_SHOT, 0, ob );
}

void dispflck( OBJECTS *ob )
{
}

void dispbird( OBJECTS *ob )
{
}

static void plnsound( OBJECTS *obp )
{
    OBJECTS *ob;

	ob = obp;
	if ( ob->ob_firing )
		sound( S_SHOT, 0, ob );
	else
		switch ( ob->ob_state ) {
			case FALLING:
				if ( ob->ob_dy >= 0 )
					sound( S_HIT, 0, ob );
				else
					sound( S_FALLING, ob->ob_y, ob );
				break;

			case FLYING:
				sound( S_PLANE, -( ob->ob_speed ), ob );
				break;

			case STALLED:
			case WOUNDED:
			case WOUNDSTALL:
				sound( S_HIT, 0, ob );
				break;

			default:
				break;
		}

}

void dispwobj( OBJECTS *obp )
{
    OBJECTS  *ob;
    OLDWDISP *ow;
    int	ox, oy, oldplot;

	ob = obp;
	ow = &wdisp[ob->ob_index];

	if ( ow->ow_xorplot )
		swpntsym( ow->ow_x, ow->ow_y, ow->ow_xorplot - 1 );

	if ( ob->ob_state >= FINISHED )
		ow->ow_xorplot = 0;
	else {
		oldplot = swpntcol( ow->ow_x = SCR_CENTR
				    + ( ob->ob_x + ob->ob_symwdt / 2 )
				    / WRLD_RSX,
				    ow->ow_y
				    = ( ob->ob_y - ob->ob_symhgt / 2 )
				    / WRLD_RSY,
				    ob->ob_owner->ob_clr );

		if ( ( oldplot == 0 ) || ( ( oldplot & 0x0003 ) == 3 ) ) {
			ow->ow_xorplot = oldplot + 1;
			return;
		}
		swpntsym( ow->ow_x, ow->ow_y, oldplot );
		ow->ow_xorplot = 0;
	}
}

static void randsd(void)
{
	if ( !( seed = seed * countmove + 7491 ) )
	{
		seed = 74917777;
	}
}

void dispwindshot(void)
{
    OBJECTS ob;

	ob.ob_type = DUMMYTYPE;
	ob.ob_symhgt = ob.ob_symwdt = 16;
	ob.ob_clr = 1;
	ob.ob_newsym = swshtsym;
	do {
		randsd();
		swputsym( (unsigned)( seed % ( SCR_WDTH - 16 ) ),
			  (unsigned)( seed % ( SCR_HGHT - 50 ) ) + 50,
			  &ob );
	} while ( --shothole );
}

void dispsplatbird(void)
{
    OBJECTS ob;

	ob.ob_type = DUMMYTYPE;
	ob.ob_symhgt = ob.ob_symwdt = 32;
	ob.ob_clr = 2;
	ob.ob_newsym = swsplsym;
	do {
		randsd();
		swputsym( (unsigned)( seed % ( SCR_WDTH - 32 ) ),
			  (unsigned)( seed % ( SCR_HGHT - 60 ) ) + 60,
			  &ob );
	} while ( --splatbird );
}

void dispoxsplat(void)
{
    assert_param(FALSE);
#if 0
    OBJECTS *ob;
    int	 i;

	swsetblk( 0,	    SCR_SEGM,
		  ( ( SCR_HGHT - SCR_MNSH - 2 ) >> 1 ) * SCR_LINW, 0xAA );
	swsetblk( SCR_ROFF, SCR_SEGM,
		  ( ( SCR_HGHT - SCR_MNSH - 3 ) >> 1 ) * SCR_LINW, 0xAA );
	splatox = 0;
	oxsplatted = 1;

	ob = nobjects;
	for ( i = 0; i < MAX_OBJS; ++i, ob++ )
		ob->ob_drwflg = ob->ob_delflg = 0;
#endif
}
