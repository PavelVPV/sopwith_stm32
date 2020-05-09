/**
 *  Audio HAL for PC Speaker emulation.
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

#ifndef SWAUDIO_HAL_H
#define SWAUDIO_HAL_H

#include <stdint.h>

/**
 * Initializes audio.
 *
 * Audio starts playing the silence immediately.
 */
void swaudio_hal_init(void);

/**
 * Plays a tone.
 *
 * The tone is played until the next call or the sound is off.
 * The relation between the tone and the frequency is:
 * Frequency = 1193280 Hz (8253 timer clock input) / duration (aka counter).
 *
 * @param duration  Determines duration between square state changes.
 */
void swaudio_hal_tone_play(uint16_t duration);

/**
 * Stops sound.
 */
void swaudio_hal_sound_off(void);

/**
 * Processes timer tick to generate new tone.
 */
void swaudio_hal_soundtick_process(void);

#endif /* SWAUDIO_HAL_H */
