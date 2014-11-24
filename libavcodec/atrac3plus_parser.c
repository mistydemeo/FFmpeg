/*
 * Copyright (C) 2016 Michael "Bazz" Bazzinotti <mbazzinotti@gmail.com>
 * Copyright (C) 2014 Maxim Poliakovski
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

#include "parser.h"
#include "get_bits.h"
#include "oma.h"

typedef struct Atrac3PlusParseContext {
    ParseContext pc;
    uint8_t hdr[8];
    int hdr_bytes_needed;
    int sample_rate, channel_id, frame_size;
    uint8_t got_bytes;
} Atrac3PlusParseContext;

static int parse_sound_frame_header(Atrac3PlusParseContext *c,
                                    const uint8_t *buf)
{
    uint16_t atrac_config;

    if (AV_RB16(buf) != 0x0FD0)
        return AVERROR_INVALIDDATA;

    atrac_config = AV_RB16(&buf[2]);
    c->sample_rate = oma_srate_tab[(atrac_config >> 13) & 7] * 100;
    c->channel_id  = (atrac_config >> 10) & 7;
    c->frame_size  = ((atrac_config & 0x3FF) * 8) + 8;

    if (!c->channel_id || !c->sample_rate || !c->frame_size)
        return AVERROR_INVALIDDATA;

    return 0;
}

static int ff_atrac3p_parse(AVCodecParserContext *s,
                 AVCodecContext *avctx,
                 const uint8_t **poutbuf, int *poutbuf_size,
                 const uint8_t *buf, int buf_size)
{
    Atrac3PlusParseContext *ctx = s->priv_data;
    const uint8_t *hdr_buf = buf;
    uint16_t bytes_remain;
    int frame_size, hdr_bytes = 8;
    int next = 0;
    int second_portion_found = 0;

    if (s->flags & PARSER_FLAG_COMPLETE_FRAMES || !buf_size) {
        next = buf_size;
    } else {
        if (buf_size >= 2) {
            bytes_remain = AV_RB16(buf);

            if (bytes_remain != 0xFD0) {
                second_portion_found = bytes_remain && !ctx->pc.index && !ctx->hdr_bytes_needed;
                /* Got something unexpected; either this means we got
                   the second part of a frame and not the first part,
                   or more likely these aren't ATRAC3+ packets in the
                   format we're expecting. For example, it might mean
                   the source data is from ATRAC3+ in RIFF WAVE, which
                   doesn't use the same format as ATRAC3+ in MPEG.
                   Just pass this along unaltered. */
                if (second_portion_found && !ctx->got_bytes) {
                    *poutbuf = buf;
                    *poutbuf_size = buf_size;
                    return buf_size;
                }

                next += 2;
                buf  += 2;
                buf_size -= 2;
                hdr_buf = buf;

                if (second_portion_found) {
                        ctx->got_bytes = 0;
                        goto process;
                }

                if (ctx->hdr_bytes_needed) {
                    if (buf_size >= ctx->hdr_bytes_needed) {
                        memcpy(&ctx->hdr[8 - ctx->hdr_bytes_needed],
                               buf, ctx->hdr_bytes_needed);
                        hdr_bytes = ctx->hdr_bytes_needed;
                        ctx->hdr_bytes_needed = 0;
                        hdr_buf = ctx->hdr;
                    }
                } else if (bytes_remain) {
                    if (buf_size < bytes_remain) {
                        av_log(avctx, AV_LOG_ERROR,
                               "Couldn't combine frame: bytes needed=%d, bytes supplied=%d\n",
                               bytes_remain, buf_size);
                    }

                    next += bytes_remain;
                    ff_combine_frame(&ctx->pc, bytes_remain, &buf, &buf_size);

                    *poutbuf      = buf;
                    *poutbuf_size = buf_size;
                    return next;
                }
            }
        }

        if (buf_size < hdr_bytes) {
            /* looks like we got an incomplete header */
            memcpy(ctx->hdr, buf, buf_size);
            ctx->hdr_bytes_needed = 8 - buf_size;
            *poutbuf = NULL;
            *poutbuf_size = 0;
            return buf_size;
        }

        if (parse_sound_frame_header(ctx, hdr_buf)) {
            av_log(avctx, AV_LOG_ERROR, "Invalid sound frame header!\n");
        }

        avctx->sample_rate    = ctx->sample_rate;
        avctx->block_align    = ctx->frame_size;
        avctx->bit_rate       = ctx->sample_rate * ctx->frame_size * 8 / 2048;
        avctx->channels       = oma_chid_to_num_channels[ctx->channel_id - 1];
        avctx->channel_layout = oma_chid_to_native_layout[ctx->channel_id - 1];

        next += hdr_bytes;
        buf  += hdr_bytes;
        buf_size -= hdr_bytes;
        if (!buf_size)
            ctx->got_bytes = 1;
    process:
        frame_size = ctx->frame_size;

        if (buf_size < frame_size)
            frame_size = END_NOT_FOUND;

        if (ff_combine_frame(&ctx->pc, frame_size, &buf, &buf_size) < 0) {
            *poutbuf = NULL;
            *poutbuf_size = 0;
            return buf_size + next;
        }

        next += frame_size >= 0 ? frame_size : buf_size;
    }

    *poutbuf      = buf;
    *poutbuf_size = buf_size;
    return next;
}

AVCodecParser ff_atrac3p_parser = {
    .codec_ids      = { AV_CODEC_ID_ATRAC3P },
    .priv_data_size = sizeof(Atrac3PlusParseContext),
    .parser_parse   = ff_atrac3p_parse,
    .parser_close   = ff_parse_close,
};
