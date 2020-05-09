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

#include "swdisplay_hal.h"

#include "main.h"
#include "sw.h"

#define COLUMN(y) ((y) * (((sFONT *)BSP_LCD_GetFont())->Width))

static uint16_t palette[] = {LCD_COLOR_BLACK, LCD_COLOR_CYAN, LCD_COLOR_MAGENTA, LCD_COLOR_WHITE};

void swdisplay_hal_init(void)
{
    uint8_t status;

    status = BSP_LCD_Init();
    assert_param(status == LCD_OK);

    BSP_LCD_DisplayOn();
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    // Clear doesn't work properly, so clear it manually
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

    BSP_LCD_SetFont(&Font8);
}

void swdisplay_hal_screen_clear(void)
{
    // BSP_LCD_Clear() doesn't work
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void swdisplay_hal_put_char(uint16_t column, uint16_t row, uint8_t ch)
{
    BSP_LCD_SetFont(&Font8);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayChar(COLUMN(column), LINE(row), ch);
}

void swdisplay_hal_put_string(uint16_t column, uint16_t row, uint8_t * p_string)
{
    BSP_LCD_SetFont(&Font8);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(COLUMN(column), LINE(row), p_string, LEFT_MODE);
}

void swdisplay_hal_set_color(uint16_t color)
{
    BSP_LCD_SetTextColor(color);
}

void swdisplay_hal_pixel_draw(int offset, char color)
{
    assert_param(offset >= 0);
    assert_param(offset <= (SCR_WDTH * SCR_HGHT));
    assert_param(color >= 0);
    assert_param(color < 4);

    uint16_t x = offset % SCR_WDTH;
    uint16_t y = offset / SCR_WDTH;
    BSP_LCD_DrawPixel(x, y, palette[(int)color]);
}

char swdisplay_hal_pixel_get(int offset)
{
    assert_param(offset >= 0);
    assert_param(offset <= (SCR_WDTH * SCR_HGHT));

    uint16_t x = offset % SCR_WDTH;
    uint16_t y = offset / SCR_WDTH;

    // Bug in FSMC or SRAM?
    // Double read fixes that
    (void) BSP_LCD_ReadPixel(x, y);
    uint16_t color = BSP_LCD_ReadPixel(x, y);

    for (size_t i = 0; i < sizeof(palette)/sizeof(palette[0]); i++)
    {
        if (color == palette[i])
        {
            return i;
        }
    }

    assert_param(FALSE);
    return 0;
}

void swdisplay_hal_pixel_xor(int offset, char color)
{
    char oldcolor = swdisplay_hal_pixel_get(offset);
    swdisplay_hal_pixel_draw(offset, oldcolor ^ color);
}

void swdisplay_hal_pixel_and(int offset, char color)
{
    char oldcolor = swdisplay_hal_pixel_get(offset);
    swdisplay_hal_pixel_draw(offset, oldcolor & color);
}

void swdisplay_hal_pixel_or(int offset, char color)
{
    char oldcolor = swdisplay_hal_pixel_get(offset);
    swdisplay_hal_pixel_draw(offset, oldcolor | color);
}
