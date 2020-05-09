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

#include "swtimer_hal.h"

#include <stdbool.h>

#include "swaudio_hal.h"
#include "main.h"
#include "swutil.h"

/** Not static. */
TIM_HandleTypeDef    g_tim_handle;

static swtimer_hal_callback * mp_timer_cb;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    mp_timer_cb();
}

void swtimer_hal_start(swtimer_hal_callback * p_cb)
{
    assert_param(p_cb);
    mp_timer_cb = p_cb;

    /* Set TIMx instance */
    g_tim_handle.Instance = TIMx;

    /* 8253 OUT0 frequency = 1.1931817 MHz / 65536 = 18.2 Hz */
    g_tim_handle.Init.Period = 100 - 1;
    g_tim_handle.Init.Prescaler = ((SystemCoreClock / 2) / 1820) - 1;
    g_tim_handle.Init.ClockDivision = 0;
    g_tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if(HAL_TIM_Base_Init(&g_tim_handle) != HAL_OK)
    {
        assert_param(false);
    }

    if(HAL_TIM_Base_Start_IT(&g_tim_handle) != HAL_OK)
    {
        assert_param(false);
    }
}
