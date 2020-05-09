/*
	swend	 -	SW end of game

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
			84-02-02	Development
			87-03-09	Microsoft compiler.
			87-03-12	Wounded airplanes.
			2003-01-27	GNU General Public License
			2020-04-10  Convert code to ANSI C
*/

#include	"sw.h"
#include <stdlib.h>

extern	int	playmode;		/* Mode of play ( SINGLE, MULTIPLE, */
					/*		  or COMPUTER )     */
extern	int	savemode;		/* Saved PC video mode		    */
extern	BOOL	hires;			/* High res debug/mono flag	    */
extern	OBJECTS *objtop;		/*  Start of object list.	    */
extern	MULTIO	*multbuff;		/*  Communications buffer	    */
extern	int	player;
extern	int	endsts[];		/* End of game status and move count*/
extern	int	endcount;
extern	BOOL	goingsun;		/* Heading for the sun flag	    */
extern	OBJECTS *objsmax;		/* Maximum object allocated	    */
extern	BOOL	repflag;		/* Report statictics flag	    */
extern	BOOL	inplay; 		/*  Currently playing flag	    */
extern	int	maxcrash;		/* Maximum number of crashes	    */

void swend( char *msg, BOOL update )
{
    char *closmsg = NULL;

	set_type( savemode );
	hires = FALSE;

	sound( 0, 0, NULL );
	swsound();

	if ( repflag )
		swreport();

#if 0
	if ( playmode == MULTIPLE )
		closmsg = multclos( update );
	else if ( playmode == ASYNCH )
		closmsg = asynclos();
#else
	assert_param( playmode == SINGLE || playmode == COMPUTER || playmode == NOVICE );
#endif

	intsoff();
//	_intterm();
	intson();
	histend();

	swputs( "\r\n" );
	if ( closmsg ) {
		swputs( closmsg );
		swputs( "\r\n" );
	}
	if ( msg ) {
		swputs( msg );
		swputs( "\r\n" );
	}

	inplay = FALSE;
	swflush();
	if ( msg || closmsg )
		exit( YES );
	else
		exit( NO );
}

void endgame( int targclr )
{
    int	 winclr;
    OBJECTS *ob;

	if ( ( ( playmode != MULTIPLE ) && ( playmode != ASYNCH ) )
		|| ( multbuff->mu_maxplyr == 1 ) )
		winclr = 1;
	else
		if ( ( objtop +1 )->ob_score == objtop->ob_score )
			winclr = 3 - targclr;
		else
			winclr = ( ( objtop + 1 )->ob_score > objtop->ob_score )
				 + 1;

	ob = objtop;
	while ( ob->ob_type == PLANE ) {
		if ( !endsts[ob->ob_index] )
		{
			if ( ( ob->ob_clr == winclr )
				  && ( ( ob->ob_crashcnt < ( MAXCRASH - 1 ) )
			       || ( ( ob->ob_crashcnt < MAXCRASH )
				  && ( ( ob->ob_state == FLYING )
				    || ( ob->ob_state == STALLED )
				    || ( ob->ob_state == WOUNDED )
				    || ( ob->ob_state == WOUNDSTALL )
				) ) ) )
			{
				winner( ob );
			}
			else
			{
				loser( ob );
			}
		}
		ob = ob->ob_next;
	}
}

void winner( OBJECTS *obp )
{
    int	 n;
    OBJECTS *ob;

	endsts[n = ( ob = obp )->ob_index] = WINNER;
	if ( n == player ) {
		endcount = 72;
		goingsun = TRUE;
		ob->ob_dx = ob->ob_dy = ob->ob_ldx = ob->ob_ldy = 0;
		ob->ob_state = FLYING;
		ob->ob_life = MAXFUEL;
		ob->ob_speed = MIN_SPEED;
	}
}

void loser( OBJECTS *ob )
{
    int	n;

	endsts[n = ob->ob_index] = LOSER;
	if ( n == player ) {
		swcolour( 0x82 );
		swposcur( 16, 12 );
		swputs( "THE END" );
		endcount = 20;
	}
}

void swreport(void)
{
	swputs( "\r\nEnd of game statictics\r\n\r\n" );
	swputs( "Objects used: " );
	dispd( ( (int) objsmax - (int) objtop + 1 ) / sizeof( OBJECTS ), 6 );
	swputs( "\r\n" );
}
