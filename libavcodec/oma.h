/*
 * Sony OpenMG (OMA) common data
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_OMA_H
#define AVCODEC_OMA_H

#include <stdint.h>

#include "internal.h"
#include "libavutil/channel_layout.h"

#define EA3_HEADER_SIZE 96
#define ID3v2_EA3_MAGIC "ea3"
#define OMA_ENC_HEADER_SIZE 16

static const uint16_t oma_srate_tab[8] = { 320, 441, 480, 882, 960, 0 };

/** map ATRAC-X channel id to internal channel layout */
static const uint64_t oma_chid_to_native_layout[7] = {
    AV_CH_LAYOUT_MONO,
    AV_CH_LAYOUT_STEREO,
    AV_CH_LAYOUT_SURROUND,
    AV_CH_LAYOUT_4POINT0,
    AV_CH_LAYOUT_5POINT1_BACK,
    AV_CH_LAYOUT_6POINT1_BACK,
    AV_CH_LAYOUT_7POINT1
};

/** map ATRAC-X channel id to total number of channels */
static const int oma_chid_to_num_channels[7] = {1, 2, 3, 4, 6, 7, 8};

#endif /* AVCODEC_OMA_H */
