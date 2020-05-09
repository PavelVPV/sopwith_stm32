/**
 *  Display HAL.
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

#ifndef SWDISPLAY_HAL_H
#define SWDISPLAY_HAL_H

#include <stdint.h>

/**
 * Initializes LCD Display.
 */
void swdisplay_hal_init(void);

/**
 * Sets the whole screen to black.
 */
void swdisplay_hal_screen_clear(void);

/**
 * Puts char at specified row and column.
 *
 * @param column    Specifies x coordinate in 8 bits step
 * @param row       Specifies y coordinate in 8 bits step
 * @param ch        Symbol to put
 */
void swdisplay_hal_put_char(uint16_t column, uint16_t row, uint8_t ch);

/**
 * Puts string at specified row and column.
 *
 * @param column    Specifies x coordinate in 8 bits step
 * @param row       Specifies y coordinate in 8 bits step
 * @param ch        Symbol to put
 */
void swdisplay_hal_put_string(uint16_t column, uint16_t row, uint8_t * p_string);

/**
 * Sets color to be used by @ref swdisplay_hal_put_string or
 * @ref swdisplay_hal_put_char.
 *
 * @param color     Set color in RGB565.
 */
void swdisplay_hal_set_color(uint16_t color);

/**
 * Put a pixel on the screen.
 *
 * Offset is calculated as y * Screen Width + x, where
 * x = 0 and y = 0 points to the top left corner of the screen.
 *
 * @param offset Pixel offset.
 * @param color  Color of the pixel.
 */
void swdisplay_hal_pixel_draw(int offset, char color);

/**
 * Get pixel at specified offset.
 *
 * @param offset Pixel offset.
 */
char swdisplay_hal_pixel_get(int offset);

/**
 * 'Xor' existing pixel's color with the new one.
 *
 * @param offset Pixel offset.
 * @param color  Pixel color.
 */
void swdisplay_hal_pixel_xor(int offset, char color);

/**
 * 'And' existing pixel's color with the new one.
 *
 * @param offset Pixel offset.
 * @param color  Pixel color.
 */
void swdisplay_hal_pixel_and(int offset, char color);

/**
 * 'Or' existing pixel's color with the new one.
 *
 * @param offset Pixel offset.
 * @param color  Pixel color.
 */
void swdisplay_hal_pixel_or(int offset, char color);

#endif /* SWDISPLAY_HAL_H */
