/*
 * Copyright (c) 2021 Loongson Technology Corporation Limited
 * Contributed by Hao Chen <chenhao@loongson.cn>
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

#include "libavutil/loongarch/cpu.h"
#include "libavutil/attributes.h"
#include "libavcodec/vp9dsp.h"
#include "vp9dsp_loongarch.h"

static av_cold void vp9dsp_mc_init_lsx(VP9DSPContext *dsp, int bpp)
{
    if (bpp == 8) {
#define init_subpel1(idx1, idx2, idxh, idxv, sz, dir, type)  \
    dsp->mc[idx1][FILTER_8TAP_SMOOTH ][idx2][idxh][idxv] =   \
        ff_##type##_8tap_smooth_##sz##dir##_lsx;             \
    dsp->mc[idx1][FILTER_8TAP_REGULAR][idx2][idxh][idxv] =   \
        ff_##type##_8tap_regular_##sz##dir##_lsx;            \
    dsp->mc[idx1][FILTER_8TAP_SHARP  ][idx2][idxh][idxv] =   \
        ff_##type##_8tap_sharp_##sz##dir##_lsx;

#define init_subpel2(idx, idxh, idxv, dir, type)      \
    init_subpel1(0, idx, idxh, idxv, 64, dir, type);  \
    init_subpel1(1, idx, idxh, idxv, 32, dir, type);  \
    init_subpel1(2, idx, idxh, idxv, 16, dir, type);  \
    init_subpel1(3, idx, idxh, idxv,  8, dir, type);  \
    init_subpel1(4, idx, idxh, idxv,  4, dir, type)

#define init_subpel3(idx, type)         \
    init_subpel2(idx, 1, 0, h, type)    \
    init_subpel2(idx, 0, 1, v, type);   \
    init_subpel2(idx, 1, 1, hv, type);

    init_subpel3(0, put);
    init_subpel3(1, avg);

#undef init_subpel1
#undef init_subpel2
#undef init_subpel3

#define init_fpel(idx1, idx2, sz, type)                                \
dsp->mc[idx1][FILTER_8TAP_SMOOTH ][idx2][0][0] = ff_##type##sz##_lsx;  \
dsp->mc[idx1][FILTER_8TAP_REGULAR][idx2][0][0] = ff_##type##sz##_lsx;  \
dsp->mc[idx1][FILTER_8TAP_SHARP  ][idx2][0][0] = ff_##type##sz##_lsx;  \
dsp->mc[idx1][FILTER_BILINEAR    ][idx2][0][0] = ff_##type##sz##_lsx

#define init_copy(idx, sz)                    \
    init_fpel(idx, 0, sz, copy);              \
    init_fpel(idx, 1, sz, avg)

    init_copy(0, 64);
    init_copy(1, 32);
    init_copy(2, 16);
    init_copy(3, 8);

#undef init_copy
#undef init_fpel
    }
}

static av_cold void vp9dsp_intrapred_init_lsx(VP9DSPContext *dsp, int bpp)
{
    if (bpp == 8) {
#define init_intra_pred_lsx(tx, sz)                             \
    dsp->intra_pred[tx][VERT_PRED]    = ff_vert_##sz##_lsx;     \
    dsp->intra_pred[tx][HOR_PRED]     = ff_hor_##sz##_lsx;      \
    dsp->intra_pred[tx][DC_PRED]      = ff_dc_##sz##_lsx;       \
    dsp->intra_pred[tx][LEFT_DC_PRED] = ff_dc_left_##sz##_lsx;  \
    dsp->intra_pred[tx][TOP_DC_PRED]  = ff_dc_top_##sz##_lsx;   \
    dsp->intra_pred[tx][DC_128_PRED]  = ff_dc_128_##sz##_lsx;   \
    dsp->intra_pred[tx][DC_127_PRED]  = ff_dc_127_##sz##_lsx;   \
    dsp->intra_pred[tx][DC_129_PRED]  = ff_dc_129_##sz##_lsx;   \
    dsp->intra_pred[tx][TM_VP8_PRED]  = ff_tm_##sz##_lsx;       \

        init_intra_pred_lsx(TX_16X16, 16x16);
        init_intra_pred_lsx(TX_32X32, 32x32);
#undef init_intra_pred_lsx

#define init_intra_pred_lsx(tx, sz)                             \
    dsp->intra_pred[tx][DC_PRED]      = ff_dc_##sz##_lsx;       \
    dsp->intra_pred[tx][LEFT_DC_PRED] = ff_dc_left_##sz##_lsx;  \
    dsp->intra_pred[tx][TOP_DC_PRED]  = ff_dc_top_##sz##_lsx;   \
    dsp->intra_pred[tx][TM_VP8_PRED]  = ff_tm_##sz##_lsx;       \

    init_intra_pred_lsx(TX_4X4, 4x4);
    init_intra_pred_lsx(TX_8X8, 8x8);
#undef init_intra_pred_lsx

    }
}

static av_cold void vp9dsp_init_lsx(VP9DSPContext *dsp, int bpp)
{
    vp9dsp_mc_init_lsx(dsp, bpp);
    vp9dsp_intrapred_init_lsx(dsp, bpp);
}

av_cold void ff_vp9dsp_init_loongarch(VP9DSPContext *dsp, int bpp)
{
    int cpu_flags = av_get_cpu_flags();
    if (have_lsx(cpu_flags))
        vp9dsp_init_lsx(dsp, bpp);
}
