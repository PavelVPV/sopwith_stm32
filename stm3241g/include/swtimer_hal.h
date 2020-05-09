/**
 *  Timer HAL for PC Timer 8253 emulation.
 *
 *  Copyright (C) 2020  Pavel Vasilyev
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SWTIMER_HAL_H
#define SWTIMER_HAL_H

/** Prototype of the timer callback. */
typedef void (swtimer_hal_callback)(void);

/**
 * Starts timer.
 *
 * It is supposed to emulate 8253 timer. The @c p_cb should be called at
 * ~18.2 Hz.
 */
void swtimer_hal_start(swtimer_hal_callback * p_cb);

#endif /* SWTIMER_HAL_H */
