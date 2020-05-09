/*

	swgrph	 -	SW screen graphics

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
			84-02-21	Development
			84-06-13	PCjr Speed-up
			85-11-05	Atari
			87-03-09	Microsoft compiler.
			2003-01-27	GNU General Public License
			2020-04-10  Convert code to ANSI C
*/

#include	"sw.h"
#include <string.h>

extern	int	displx, disprx; 	/* Display left and right bounds    */
extern	int	dispdx; 		/* Display shift		    */
extern  char    videoram[];     /* Main display area */
extern	GRNDTYPE ground[];		/* Ground height by pixel	    */
extern	BOOL	dispinit;		/* Initalized display flag.	    */
extern	OBJECTS *objtop;		/* Top of object list		    */
extern	OBJECTS *deltop;		/* Newly deallocated objects list   */
extern	int	forcdisp;		/* Force display of ground	    */

extern char	*dispoff;		/* Current display offset	    */
static	GRNDTYPE grndsave[SCR_WDTH];	/* Saved ground buffer for last     */
					/*   last display		    */
static	void	( *dispg )( GRNDTYPE * );		/* display ground routine (mono,clr)*/
static	void	( *drawpnt )( int x, int y, int clr, int *oldclr ); 	/* draw point routine		    */
static	void	( *drawsym )( OBJECTS *ob, int x, int y, char *symbol, int clr, int *retcode ); 	/* draw symbol routine		    */

static void dispgrnd(void);
static void dispgm( GRNDTYPE *gptr );
static void drawsm( OBJECTS *ob, int x, int y, char * symbol, int clr, int *retcode );

/*---------------------------------------------------------------------------

	Main display loop.   Delete and display all visible objects.
	Delete any newly deleted objects

---------------------------------------------------------------------------*/

void swdisp(void)
{
    OBJECTS *ob;

	setvdisp();
	for ( ob = objtop; ob; ob = ob->ob_next ) {
		if ( ( !( ob->ob_delflg && ob->ob_drwflg ) )
			|| ( ob->ob_symhgt == 1 )
			|| ( ob->ob_oldsym != ob->ob_newsym )
			|| ( ob->ob_y != ob->ob_oldy )
			|| ( ( ob->ob_oldx + displx ) != ob->ob_x ) ) {
			if ( ob->ob_delflg )
				( *drawsym )( ob, ob->ob_oldx, ob->ob_oldy,
					      ob->ob_oldsym, ob->ob_clr, NULL );
			if ( !ob->ob_drwflg )
				continue;
			if ( ( ob->ob_x < displx ) || ( ob->ob_x > disprx ) ) {
				ob->ob_drwflg = 0;
				continue;
			}
			( *drawsym )( ob, (ob->ob_oldx = ob->ob_x - displx),
				      (ob->ob_oldy = ob->ob_y),
				      ob->ob_newsym,
				      ob->ob_clr, NULL );
		}
		if ( ob->ob_drawf )
			( *( ob->ob_drawf ) )( ob );
	}

	for ( ob = deltop; ob; ob = ob->ob_next )
		if ( ob->ob_delflg )
			( *drawsym )( ob, ob->ob_oldx, ob->ob_oldy,
				      ob->ob_oldsym, ob->ob_clr, NULL );

	dispgrnd();
	dispinit = FALSE;
	forcdisp = FALSE;
}

/*---------------------------------------------------------------------------

	Update display of ground.   Delete previous display of ground by
	XOR graphics.

	Different routines are used to display/delete ground on colour
	or monochrome systems.

---------------------------------------------------------------------------*/

static void dispgrnd(void)
{
	if ( !dispinit ) {
		if ( !( dispdx || forcdisp ) )
			return;
		( *dispg )( grndsave );
	}
	movmem( ground + displx, grndsave, SCR_WDTH * sizeof( GRNDTYPE ) );
	( *dispg )( ground + displx );
}

static void dispgm( GRNDTYPE *gptr )
{
    GRNDTYPE *g, gl, gc;
    int	gmask, i;
    int sptr;

	i = SCR_WDTH;
    gl = *( g = gptr );
    gmask = 0x3;
    sptr = ( SCR_HGHT - gl - 1 ) * 320;

    while ( i-- ) {
        if ( gl == ( gc = *g++ ) ) {
            swpxlxor(sptr, gmask);
        } else if ( gl < gc )
            do  {
                sptr-=320;
                swpxlxor(sptr, gmask);
            } while ( ++gl < gc );
        else
        {
            do  {
                sptr+=320;
                swpxlxor(sptr, gmask);
            } while ( --gl > gc );
        }

        ++sptr;
    }
}

/*---------------------------------------------------------------------------

	External display ground call for title screen processing.

---------------------------------------------------------------------------*/

void swground(void)
{
	dispgrnd();
}

/*---------------------------------------------------------------------------

	Clear the collision detection portion of auxiliary video ram

---------------------------------------------------------------------------*/

void swclrcol(void)
{
    char *sptr;
    int	l;

    assert_param(dispoff != 0);
    memset(videoram, 0, SCR_WDTH * SCR_HGHT);
}

/*---------------------------------------------------------------------------

	Display an object's current symbol at a specified screen location
	Collision detection may or may not be asked for.

	Different routines are used to display symbols on colour or
	monochrome systems.

---------------------------------------------------------------------------*/

void swputsym( int x, int y, OBJECTS *ob )
{
	( *drawsym )( ob, x, y, ob->ob_newsym, ob->ob_clr, NULL );
}

int swputcol( int x, int y, OBJECTS *ob )
{
int	retcode = FALSE;

	( *drawsym )( ob, x, y, ob->ob_newsym, ob->ob_clr, &retcode );
	return( retcode );
}

char	fill[] = {
0x00,0x03,0x03,0x03,0x0C,0x0F,0x0F,0x0F,0x0C,0x0F,0x0F,0x0F,0x0C,0x0F,0x0F,0x0F,
0x30,0x33,0x33,0x33,0x3C,0x3F,0x3F,0x3F,0x3C,0x3F,0x3F,0x3F,0x3C,0x3F,0x3F,0x3F,
0x30,0x33,0x33,0x33,0x3C,0x3F,0x3F,0x3F,0x3C,0x3F,0x3F,0x3F,0x3C,0x3F,0x3F,0x3F,
0x30,0x33,0x33,0x33,0x3C,0x3F,0x3F,0x3F,0x3C,0x3F,0x3F,0x3F,0x3C,0x3F,0x3F,0x3F,
0xC0,0xC3,0xC3,0xC3,0xCC,0xCF,0xCF,0xCF,0xCC,0xCF,0xCF,0xCF,0xCC,0xCF,0xCF,0xCF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xC0,0xC3,0xC3,0xC3,0xCC,0xCF,0xCF,0xCF,0xCC,0xCF,0xCF,0xCF,0xCC,0xCF,0xCF,0xCF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xC0,0xC3,0xC3,0xC3,0xCC,0xCF,0xCF,0xCF,0xCC,0xCF,0xCF,0xCF,0xCC,0xCF,0xCF,0xCF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,
0xF0,0xF3,0xF3,0xF3,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF,0xFC,0xFF,0xFF,0xFF
};

extern int player;
static void drawsm( OBJECTS *ob, int x, int y, char * symbol, int clr, int *retcode )
{
	char    *sym;
	int s, sptr;
    int j, c, cr, pc;
    int wdth, wrap, n;

    if ( !( sym = symbol ) )
        return;

    if (ob->ob_type == BOMB)
    {
        volatile uint8_t i = 0;
        while (i);
    }

    if ( ( ob->ob_symhgt == 1 ) && ( ob->ob_symwdt == 1 ) ) {
        drawpm( x, y, (int) sym, retcode );
        return;
    }

    // Each byte contains 4 pixels
    wdth = ob->ob_symwdt;
    wrap = x - SCR_WDTH + wdth;

    if (wrap > 0)
    {
        wdth -= wrap;
    }

    // Each byte contains 4 pixels, but
    // symbol is wrapped by bytes, not pixels.
    // E.g. if one symbol should be by 5 pixels (1 byte, 2 bits),
    // the symbol data should be wrapped by 1 byte.
    wrap >>= 2;

    if ( ( n = ob->ob_symhgt ) > ( y + 1 ) )
        n = y + 1;
    sptr = ( ( SCR_HGHT - y - 1 ) * 320 ) + x;

    while ( n-- ) {
        s = sptr;
        j = wdth;
        pc = 0;
        // Stratch 'stratches' one byte to 4 pixels.
        // 2 bits per pixel.
        char stratch = 8;
        while ( j-- ) {
            char quarter = (*sym >> (stratch - 2)) & 0x3;

            stratch -= 2;
            if (!stratch) {
                sym++;
                stratch = 8;
            }

            // Invert colors for target and computer plane
            if ( ( ob->ob_type == TARGET || ob->ob_type == PLANE )
                && ob->ob_owner->ob_index != player)
            {
                if (quarter == 2)
                {
                    quarter = 1;
                }
                else if (quarter == 1)
                {
                    quarter = 2;
                }
            }

            cr = ( c = quarter );
            pc = cr;
            if ( retcode && ( swpxlget(s) & fill[c & 0x00FF] ) ){
                *retcode = TRUE;
                retcode = 0;
            }
            swpxlxor(s, c);
            s++;
        }
        if ( wrap >= 0 )
            sym += wrap;
        else {
            if ( retcode && ( swpxlget(s) & fill[pc & 0x00FF ] ) ){
                *retcode = TRUE;
                retcode = 0;
            }
            swpxlxor(s, pc);
        }
        sptr += 320;
    }
}

/*---------------------------------------------------------------------------

	External calls to display a point of a specified colour at a
	specified position.   The point request may or may not ask for
	collision detection by returning the old colour of the point.

	Different routines are used to display points on colour or
	monochrome systems.

---------------------------------------------------------------------------*/

void swpntsym( int x, int y, int clr )
{
	( *drawpnt )( x, y, clr, NULL );
}

int swpntcol( int x, int y, int clr )
{
    int	oldclr;

	( *drawpnt )( x, y, clr, &oldclr );
	return( oldclr );
}

void drawpc( int x, int y, int clr, int *oldclr )
{
    int c, mask;
    char *sptr;

	sptr = dispoff + ( ( SCR_HGHT - y - 1 ) * 160 )
		       + ( ( x & 0xFFF0 ) >> 1 )
		       + ( ( x & 0x0008 ) >> 3 );
	mask = 0x80 >> ( x &= 0x0007 );

	if ( oldclr ) {
		c = ( *sptr & mask )
			| ( ( *( sptr+2 ) & mask ) << 1 )
			| ( ( *( sptr+4 ) & mask ) << 2 )
			| ( ( *( sptr+6 ) & mask ) << 3 );
		*oldclr = ( c >> ( 7 - x ) ) & 0x00FF;
	}

	c = clr << ( 7 - x );
	if ( clr & 0x0080 ) {
		*sptr	    ^= ( mask & c );
		*( sptr+2 ) ^= ( mask & ( c >> 1 ) );
		*( sptr+4 ) ^= ( mask & ( c >> 2 ) );
		*( sptr+6 ) ^= ( mask & ( c >> 3 ) );
	} else {
		mask = ~mask;
		*sptr	    &= mask;
		*( sptr+2 ) &= mask;
		*( sptr+4 ) &= mask;
		*( sptr+6 ) &= mask;

		mask = ~mask;
		*sptr	    |= ( mask & c );
		*( sptr+2 ) |= ( mask & ( c >> 1 ) );
		*( sptr+4 ) |= ( mask & ( c >> 2 ) );
		*( sptr+6 ) |= ( mask & ( c >> 3 ) );
	}
}

void drawpm( int x, int y, int clr, int *oldclr )
{
    int c, mask;
    int sptr;

    sptr = ( ( SCR_HGHT - y - 1 ) * 320 ) + x;
    mask = 3;

    if ( oldclr )
    {
        *oldclr = swpxlget(sptr) & mask;
    }

    if ( clr & 0x80 ) { // Shot
        swpxlxor(sptr, ( mask & clr ));
    } else {
        swpxland(sptr, ~mask);
        swpxlor(sptr, ( mask & clr ));
    }
}

/*---------------------------------------------------------------------------

	Get/set the current screen resolution.	The resolution is never
	changed on a monochrome system.  Low-res 16 colour is used on
	colour systems, (equivalent to IBM type 4), except in debugging
	instances where high-res 4 colour is used. (equivalent to IBM type 6)

	On colour systems, the pixel map for each symbol is converted to
	optomize video ram updates.  The bit pattern abcdefghijklmnop is
	converted to bdfhjlnpacegikmo for all words of all symbols.
---------------------------------------------------------------------------*/

int get_type(void)
{
    // The value doesn't matter.
	return 42;
}

void set_type( int type )
{
	(void)type;

    dispg = dispgm;
    drawpnt = drawpm;
    drawsym = drawsm;
}

/*---------------------------------------------------------------------------

	External calls to specify current video ram as screen ram or
	auxiliary screen area.

---------------------------------------------------------------------------*/

void setvdisp(void)
{
	dispoff = NULL;
}

void setadisp(void)
{
    dispoff = videoram;
}
