/**
 *  Buttons HAL.
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

#include "swbuttons_hal.h"

#include "main.h"
#include "sw.h"

void swbuttons_hal_init(void)
{
    uint8_t status;

    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
    BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
    BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_GPIO);

    /* My board was asserting here most of the time so I had to re-plug the power supply. */
    status = BSP_JOY_Init(JOY_MODE_GPIO);
    assert_param(status == IO_OK);
}

int swgetjoy(void)
{
    struct {
        JOYState_TypeDef state;
        int action;
    } joystick[] = {
        {JOY_UP,    K_FLAPU},
        {JOY_DOWN,  K_FLAPD},
        {JOY_LEFT,  K_DEACC},
        {JOY_RIGHT, K_ACCEL},
        {JOY_SEL,   K_BOMB},
    };

    struct {
        Button_TypeDef button;
        GPIO_PinState state;
        int action;
    } buttons[] = {
        {BUTTON_KEY,    GPIO_PIN_RESET, K_SHOT},
        {BUTTON_TAMPER, GPIO_PIN_RESET, K_FLIP},
        {BUTTON_WAKEUP, GPIO_PIN_SET,   K_HOME},

        /* Not enough buttons on the board :(. */
//        {BUTTON_WAKEUP, GPIO_PIN_SET,   K_BREAK},
//        {BUTTON_WAKEUP, GPIO_PIN_SET,   K_MISSILE},
//        {BUTTON_WAKEUP, GPIO_PIN_SET,   K_STARBURST},
    };

    JOYState_TypeDef JoyState = BSP_JOY_GetState();
    for (size_t i = 0; i < ARRAY_SIZE(joystick); i++)
    {
        if (JoyState == joystick[i].state)
        {
            return joystick[i].action;
        }
    }

    for (size_t i = 0; i < ARRAY_SIZE(buttons); i++)
    {
        GPIO_PinState state = BSP_PB_GetState(buttons[i].button);

        if (state == buttons[i].state)
        {
            return buttons[i].action;
        }
    }

    return 0;
}
