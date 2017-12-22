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

#ifndef AVFORMAT_OMA_H
#define AVFORMAT_OMA_H

#include "internal.h"

enum {
    OMA_CODECID_ATRAC3  = 0,
    OMA_CODECID_ATRAC3P = 1,
    OMA_CODECID_MP3     = 3,
    OMA_CODECID_LPCM    = 4,
    OMA_CODECID_WMA     = 5,
    OMA_CODECID_ATRAC3PAL = 33,
    OMA_CODECID_ATRAC3AL  = 34,
};

static const AVCodecTag oma_codec_tags[] = {
    { AV_CODEC_ID_ATRAC3,      OMA_CODECID_ATRAC3    },
    { AV_CODEC_ID_ATRAC3P,     OMA_CODECID_ATRAC3P   },
    { AV_CODEC_ID_MP3,         OMA_CODECID_MP3       },
    { AV_CODEC_ID_PCM_S16BE,   OMA_CODECID_LPCM      },
    { AV_CODEC_ID_ATRAC3PAL,   OMA_CODECID_ATRAC3PAL },
    { AV_CODEC_ID_ATRAC3AL,    OMA_CODECID_ATRAC3AL  },
    { 0 },
};

#endif /* AVFORMAT_OMA_H */
