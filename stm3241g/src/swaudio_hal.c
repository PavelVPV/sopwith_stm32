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

#include "swaudio_hal.h"

#include <stdbool.h>
#include <string.h>

#include "swtimer_hal.h"
#include "main.h"
#include "swsound.h"

/** x86 8253 timer clock frequency 1.1931817 MHz. */
#define TIMER_FREQ 1193280

/**
 * Number of periods that the buffer should be able to keep at the
 * corresponding frequency.
 *
 * Should be multiple of 2 to keep at least one pair of low and high values.
 */
#define SQUARE_WAVE_PERIODS_PER_BUFFER 4

/**
 * 48KHz is used because with less frequency the speaker generates sound
 * noise in background.
 */
#define AUDIO_FREQ I2S_AUDIOFREQ_48K

/**
 * Buffer to be used to store square wave. Longer buffer generates
 * less clicks in the sound.
 */
static uint16_t m_square_wave[2638 * 3];

/**
 * Determines the length of the generated square wave.
 * The square wave length varies by the frequency it is generated from.
 * Needs by audio hw to cycle the sound while the lenght is change when anew
 * tone is generated.
 */
static uint16_t m_square_wave_length = 0;

/** Generates square wave. Returns the lenght of the generated wave. */
static size_t square_wave_generate(uint16_t *p_buf, size_t len, size_t freq)
{
    static uint16_t meandr = INT16_MAX;

    size_t req_buf_size = (size_t)( AUDIO_FREQ / ( freq / 2 ) ) * SQUARE_WAVE_PERIODS_PER_BUFFER;

    /* Uncomment to adjust the square wave buffer size.
     * The bigger size helps to get rid of clicks in the sound.
     * The buffer should be long enough to fit at least
     * */
    /* assert_param(req_buf_size <= len); */
    if (req_buf_size > len)
    {
        req_buf_size = len;
    }

    memset(m_square_wave, 0, ARRAY_SIZE(m_square_wave));

    for (size_t i = 0; i < req_buf_size; i++)
    {
        if ( !( i % ( AUDIO_FREQ / ( freq / 2 ) ) ) )
        {
            if (meandr == INT16_MAX)
            {
                meandr = 0;
            }
            else
            {
                meandr = INT16_MAX;
            }
        }

        p_buf[i] = meandr;
    }

    return req_buf_size;
}

/** Calculates the remaining file size and new position of the pointer. */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
    BSP_AUDIO_OUT_ChangeBuffer(m_square_wave, m_square_wave_length);
}

/** Manages the DMA Half Transfer complete interrupt. */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
    /* No-op */
}

/** Manages the DMA FIFO error event. */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
    assert_param(false);
}

void swaudio_hal_init(void)
{
    uint8_t status;

    memset(m_square_wave, 0, ARRAY_SIZE(m_square_wave));

    status = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_SPEAKER, 70, AUDIO_FREQ);
    assert_param(status == AUDIO_OK);

    status = BSP_AUDIO_OUT_Play(m_square_wave, m_square_wave_length);
    assert_param(status == AUDIO_OK);
}

void swaudio_hal_tone_play(uint16_t duration)
{
    if (!duration)
    {
        memset(m_square_wave, 0, ARRAY_SIZE(m_square_wave));
        m_square_wave_length = ARRAY_SIZE(m_square_wave);
    }
    else
    {
        /* This describes relations between the 8253 timer clock input frequency, counter value (duration)
         * and the desired frequency. */
        size_t freq = (TIMER_FREQ / (size_t) duration);
        m_square_wave_length = square_wave_generate(m_square_wave, ARRAY_SIZE(m_square_wave), freq);
    }

    BSP_AUDIO_OUT_ChangeBuffer(m_square_wave, m_square_wave_length);
}

void swaudio_hal_sound_off(void)
{
    memset(m_square_wave, 0, ARRAY_SIZE(m_square_wave));
    m_square_wave_length = ARRAY_SIZE(m_square_wave);

    BSP_AUDIO_OUT_ChangeBuffer(m_square_wave, m_square_wave_length);
}

void swaudio_hal_soundtick_process(void)
{
    soundadj();
}
