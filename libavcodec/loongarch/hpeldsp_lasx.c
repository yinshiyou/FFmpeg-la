/*
 * Copyright (c) 2021 Loongson Technology Corporation Limited
 * Contributed by Shiyou Yin <yinshiyou-hf@loongson.cn>
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

#include "libavutil/loongarch/generic_macros_lasx.h"
#include "libavcodec/loongarch/hpeldsp_lasx.h"

static inline void
put_pixels8_l2_8_lsx(uint8_t *dst, const uint8_t *src1, const uint8_t *src2,
                     int dst_stride, int src_stride1, int src_stride2, int h)
{
    __asm__ volatile (
        "1:                                       \n\t"
        "vld      $vr0,    %[src1], 0             \n\t"
        "add.d    %[src1], %[src1], %[srcStride1] \n\t"
        "vld      $vr1,    %[src1], 0             \n\t"
        "add.d    %[src1], %[src1], %[srcStride1] \n\t"
        "vld      $vr2,    %[src1], 0             \n\t"
        "add.d    %[src1], %[src1], %[srcStride1] \n\t"
        "vld      $vr3,    %[src1], 0             \n\t"
        "add.d    %[src1], %[src1], %[srcStride1] \n\t"

        "vld      $vr4,    %[src2], 0             \n\t"
        "add.d    %[src2], %[src2], %[srcStride2] \n\t"
        "vld      $vr5,    %[src2], 0             \n\t"
        "add.d    %[src2], %[src2], %[srcStride2] \n\t"
        "vld      $vr6,    %[src2], 0             \n\t"
        "add.d    %[src2], %[src2], %[srcStride2] \n\t"
        "vld      $vr7,    %[src2], 0             \n\t"
        "add.d    %[src2], %[src2], %[srcStride2] \n\t"

        "addi.d   %[h],    %[h],    -4            \n\t"

        "vavgr.bu $vr0,    $vr4,    $vr0          \n\t"
        "vavgr.bu $vr1,    $vr5,    $vr1          \n\t"
        "vavgr.bu $vr2,    $vr6,    $vr2          \n\t"
        "vavgr.bu $vr3,    $vr7,    $vr3          \n\t"
        "vstelm.d $vr0,    %[dst],  0,  0         \n\t"
        "add.d    %[dst],  %[dst],  %[dstStride]  \n\t"
        "vstelm.d $vr1,    %[dst],  0,  0         \n\t"
        "add.d    %[dst],  %[dst],  %[dstStride]  \n\t"
        "vstelm.d $vr2,    %[dst],  0,  0         \n\t"
        "add.d    %[dst],  %[dst],  %[dstStride]  \n\t"
        "vstelm.d $vr3,    %[dst],  0,  0         \n\t"
        "add.d    %[dst],  %[dst],  %[dstStride]  \n\t"
        "bnez     %[h],             1b            \n\t"

        : [dst]"+&r"(dst), [src2]"+&r"(src2), [src1]"+&r"(src1),
          [h]"+&r"(h)
        : [dstStride]"r"(dst_stride), [srcStride1]"r"(src_stride1),
          [srcStride2]"r"(src_stride2)
        : "memory"
    );
}

static inline void
put_pixels16_l2_8_lsx(uint8_t *dst, const uint8_t *src1, const uint8_t *src2,
                      int dst_stride, int src_stride1, int src_stride2, int h)
{
    __asm__ volatile (
        "1:                                      \n\t"
        "vld     $vr0,    %[src1], 0             \n\t"
        "add.d   %[src1], %[src1], %[srcStride1] \n\t"
        "vld     $vr1,    %[src1], 0             \n\t"
        "add.d   %[src1], %[src1], %[srcStride1] \n\t"
        "vld     $vr2,    %[src1], 0             \n\t"
        "add.d   %[src1], %[src1], %[srcStride1] \n\t"
        "vld     $vr3,    %[src1], 0             \n\t"
        "add.d   %[src1], %[src1], %[srcStride1] \n\t"

        "vld     $vr4,    %[src2], 0             \n\t"
        "add.d   %[src2], %[src2], %[srcStride2] \n\t"
        "vld     $vr5,    %[src2], 0             \n\t"
        "add.d   %[src2], %[src2], %[srcStride2] \n\t"
        "vld     $vr6,    %[src2], 0             \n\t"
        "add.d   %[src2], %[src2], %[srcStride2] \n\t"
        "vld     $vr7,    %[src2], 0             \n\t"
        "add.d   %[src2], %[src2], %[srcStride2] \n\t"

        "addi.d  %[h],    %[h],    -4            \n\t"

        "vavgr.bu $vr0,   $vr4,    $vr0          \n\t"
        "vavgr.bu $vr1,   $vr5,    $vr1          \n\t"
        "vavgr.bu $vr2,   $vr6,    $vr2          \n\t"
        "vavgr.bu $vr3,   $vr7,    $vr3          \n\t"
        "vst     $vr0,    %[dst],  0             \n\t"
        "add.d   %[dst],  %[dst],  %[dstStride]  \n\t"
        "vst     $vr1,    %[dst],  0             \n\t"
        "add.d   %[dst],  %[dst],  %[dstStride]  \n\t"
        "vst     $vr2,    %[dst],  0             \n\t"
        "add.d   %[dst],  %[dst],  %[dstStride]  \n\t"
        "vst     $vr3,    %[dst],  0             \n\t"
        "add.d   %[dst],  %[dst],  %[dstStride]  \n\t"
        "bnez    %[h],             1b            \n\t"

        : [dst]"+&r"(dst), [src2]"+&r"(src2), [src1]"+&r"(src1),
          [h]"+&r"(h)
        : [dstStride]"r"(dst_stride), [srcStride1]"r"(src_stride1),
          [srcStride2]"r"(src_stride2)
        : "memory"
    );
}

void ff_put_pixels8_8_lasx(uint8_t *block, const uint8_t *pixels,
                           ptrdiff_t line_size, int h)
{
    uint64_t tmp[8];
    int h_8 = h >> 3;
    int res = h & 7;

    __asm__ volatile (
        "beqz       %[h_8],                2f          \n\t"
        "1:                                            \n\t"
        "ld.d       %[tmp0],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp1],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp2],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp3],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp4],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp5],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp6],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "ld.d       %[tmp7],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"

        "addi.d     %[h_8],     %[h_8],    -1          \n\t"

        "st.d       %[tmp0],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp1],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp2],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp3],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp4],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp5],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp6],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "st.d       %[tmp7],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "bnez       %[h_8],     1b                     \n\t"

        "2:                                            \n\t"
        "beqz       %[res],     4f                     \n\t"
        "3:                                            \n\t"
        "ld.d       %[tmp0],    %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "addi.d     %[res],     %[res],    -1          \n\t"
        "st.d       %[tmp0],    %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "bnez       %[res],     3b                     \n\t"
        "4:                                            \n\t"
        : [tmp0]"=&r"(tmp[0]),        [tmp1]"=&r"(tmp[1]),
          [tmp2]"=&r"(tmp[2]),        [tmp3]"=&r"(tmp[3]),
          [tmp4]"=&r"(tmp[4]),        [tmp5]"=&r"(tmp[5]),
          [tmp6]"=&r"(tmp[6]),        [tmp7]"=&r"(tmp[7]),
          [dst]"+&r"(block),          [src]"+&r"(pixels),
          [h_8]"+&r"(h_8),            [res]"+&r"(res)
        : [stride]"r"(line_size)
        : "memory"
    );
}

void ff_put_pixels16_8_lsx(uint8_t *block, const uint8_t *pixels,
                           ptrdiff_t line_size, int h)
{
    int h_8 = h >> 3;
    int res = h & 7;

    __asm__ volatile (
        "beqz       %[h_8],     2f                     \n\t"
        "1:                                            \n\t"
        "vld        $vr0,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr1,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr2,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr3,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr4,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr5,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr6,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "vld        $vr7,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"

        "addi.d     %[h_8],     %[h_8],    -1          \n\t"

        "vst        $vr0,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr1,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr2,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr3,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr4,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr5,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr6,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "vst        $vr7,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "bnez       %[h_8],     1b                     \n\t"

        "2:                                            \n\t"
        "beqz       %[res],     4f                     \n\t"
        "3:                                            \n\t"
        "vld        $vr0,       %[src],    0x0         \n\t"
        "add.d      %[src],     %[src],    %[stride]   \n\t"
        "addi.d     %[res],     %[res],    -1          \n\t"
        "vst        $vr0,       %[dst],    0x0         \n\t"
        "add.d      %[dst],     %[dst],    %[stride]   \n\t"
        "bnez       %[res],     3b                     \n\t"
        "4:                                            \n\t"
        : [dst]"+&r"(block),          [src]"+&r"(pixels),
          [h_8]"+&r"(h_8),            [res]"+&r"(res)
        : [stride]"r"(line_size)
        : "memory"
    );
}

void ff_put_pixels8_x2_8_lasx(uint8_t *block, const uint8_t *pixels,
                              ptrdiff_t line_size, int h)
{
    put_pixels8_l2_8_lsx(block, pixels, pixels + 1, line_size, line_size,
                         line_size, h);
}

void ff_put_pixels8_y2_8_lasx(uint8_t *block, const uint8_t *pixels,
                              ptrdiff_t line_size, int h)
{
    put_pixels8_l2_8_lsx(block, pixels, pixels + line_size, line_size,
                         line_size, line_size, h);
}

void ff_put_pixels16_x2_8_lasx(uint8_t *block, const uint8_t *pixels,
                               ptrdiff_t line_size, int h)
{
    put_pixels16_l2_8_lsx(block, pixels, pixels + 1, line_size, line_size,
                          line_size, h);
}

void ff_put_pixels16_y2_8_lasx(uint8_t *block, const uint8_t *pixels,
                               ptrdiff_t line_size, int h)
{
    put_pixels16_l2_8_lsx(block, pixels, pixels + line_size, line_size,
                          line_size, line_size, h);
}

static void common_hz_bil_no_rnd_16x16_lasx(const uint8_t *src,
                                            int32_t src_stride,
                                            uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    int32_t src_stride_4x = src_stride << 2;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    src += src_stride_4x;
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 0);
    __lasx_xvstelm_d(src1, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 2);
    __lasx_xvstelm_d(src1, dst + 8, 0, 3);
    dst += dst_stride;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    src += src_stride_4x;
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 0);
    __lasx_xvstelm_d(src1, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 2);
    __lasx_xvstelm_d(src1, dst + 8, 0, 3);
    dst += dst_stride;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    src += src_stride_4x;
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 0);
    __lasx_xvstelm_d(src1, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 2);
    __lasx_xvstelm_d(src1, dst + 8, 0, 3);
    dst += dst_stride;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    src += src_stride_4x;
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 0);
    __lasx_xvstelm_d(src1, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 2);
    __lasx_xvstelm_d(src1, dst + 8, 0, 3);
}

static void common_hz_bil_no_rnd_8x16_lasx(const uint8_t *src,
                                           int32_t src_stride,
                                           uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    int32_t src_stride_4x = src_stride << 2;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    src += src_stride_4x;
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 0);
    __lasx_xvstelm_d(src1, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 2);
    __lasx_xvstelm_d(src1, dst + 8, 0, 3);
    dst += dst_stride;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    src += src_stride_4x;
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 0);
    __lasx_xvstelm_d(src1, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src1, dst, 0, 2);
    __lasx_xvstelm_d(src1, dst + 8, 0, 3);
}

void ff_put_no_rnd_pixels16_x2_8_lasx(uint8_t *block, const uint8_t *pixels,
                                      ptrdiff_t line_size, int h)
{
    if (h == 16) {
        common_hz_bil_no_rnd_16x16_lasx(pixels, line_size, block, line_size);
    } else if (h == 8) {
        common_hz_bil_no_rnd_8x16_lasx(pixels, line_size, block, line_size);
    }
}

static void common_vt_bil_no_rnd_16x16_lasx(const uint8_t *src,
                                            int32_t src_stride,
                                            uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7, src8;
    __m256i src9, src10, src11, src12, src13, src14, src15, src16;
    int32_t src_stride_8x = src_stride << 3;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    src += src_stride_8x;
    LASX_LD_8(src, src_stride, src8, src9, src10, src11, src12, src13, src14, src15);
    src += src_stride_8x;
    src16 = LASX_LD(src);

    LASX_PCKEV_Q_8(src1, src0, src2, src1, src3, src2, src4, src3,
                   src5, src4, src6, src5, src7, src6, src8, src7,
                   src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_PCKEV_Q_8(src9, src8, src10, src9, src11, src10, src12, src11,
                   src13, src12, src14, src13, src15, src14, src16, src15,
                   src8, src9, src10, src11, src12, src13, src14, src15);
    src0  = __lasx_xvavg_bu(src0, src1);
    src2  = __lasx_xvavg_bu(src2, src3);
    src4  = __lasx_xvavg_bu(src4, src5);
    src6  = __lasx_xvavg_bu(src6, src7);
    src8  = __lasx_xvavg_bu(src8, src9);
    src10 = __lasx_xvavg_bu(src10, src11);
    src12 = __lasx_xvavg_bu(src12, src13);
    src14 = __lasx_xvavg_bu(src14, src15);

    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src2, dst, 0, 0);
    __lasx_xvstelm_d(src2, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src2, dst, 0, 2);
    __lasx_xvstelm_d(src2, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src4, dst, 0, 0);
    __lasx_xvstelm_d(src4, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src4, dst, 0, 2);
    __lasx_xvstelm_d(src4, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src6, dst, 0, 0);
    __lasx_xvstelm_d(src6, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src6, dst, 0, 2);
    __lasx_xvstelm_d(src6, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src8, dst, 0, 0);
    __lasx_xvstelm_d(src8, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src8, dst, 0, 2);
    __lasx_xvstelm_d(src8, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src10, dst, 0, 0);
    __lasx_xvstelm_d(src10, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src10, dst, 0, 2);
    __lasx_xvstelm_d(src10, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src12, dst, 0, 0);
    __lasx_xvstelm_d(src12, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src12, dst, 0, 2);
    __lasx_xvstelm_d(src12, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src14, dst, 0, 0);
    __lasx_xvstelm_d(src14, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src14, dst, 0, 2);
    __lasx_xvstelm_d(src14, dst + 8, 0, 3);
}

static void common_vt_bil_no_rnd_8x16_lasx(const uint8_t *src,
                                           int32_t src_stride,
                                           uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7, src8;
    int32_t src_stride_8x = src_stride << 3;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    src += src_stride_8x;
    src8 = LASX_LD(src);

    LASX_PCKEV_Q_8(src1, src0, src2, src1, src3, src2, src4, src3,
                   src5, src4, src6, src5, src7, src6, src8, src7,
                   src0, src1, src2, src3, src4, src5, src6, src7);
    src0  = __lasx_xvavg_bu(src0, src1);
    src2  = __lasx_xvavg_bu(src2, src3);
    src4  = __lasx_xvavg_bu(src4, src5);
    src6  = __lasx_xvavg_bu(src6, src7);

    __lasx_xvstelm_d(src0, dst, 0, 0);
    __lasx_xvstelm_d(src0, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src0, dst, 0, 2);
    __lasx_xvstelm_d(src0, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src2, dst, 0, 0);
    __lasx_xvstelm_d(src2, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src2, dst, 0, 2);
    __lasx_xvstelm_d(src2, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src4, dst, 0, 0);
    __lasx_xvstelm_d(src4, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src4, dst, 0, 2);
    __lasx_xvstelm_d(src4, dst + 8, 0, 3);
    dst += dst_stride;
    __lasx_xvstelm_d(src6, dst, 0, 0);
    __lasx_xvstelm_d(src6, dst + 8, 0, 1);
    dst += dst_stride;
    __lasx_xvstelm_d(src6, dst, 0, 2);
    __lasx_xvstelm_d(src6, dst + 8, 0, 3);
}

void ff_put_no_rnd_pixels16_y2_8_lasx(uint8_t *block, const uint8_t *pixels,
                                      ptrdiff_t line_size, int h)
{
    if (h == 16) {
        common_vt_bil_no_rnd_16x16_lasx(pixels, line_size, block, line_size);
    } else if (h == 8) {
        common_vt_bil_no_rnd_8x16_lasx(pixels, line_size, block, line_size);
    }
}

static void common_hv_bil_no_rnd_16x16_lasx(const uint8_t *src,
                                            int32_t src_stride,
                                            uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7, src8, src9;
    __m256i src10, src11, src12, src13, src14, src15, src16, src17;
    __m256i sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7;
    int32_t src_stride_8x = src_stride << 3;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_LD_8((src + 1), src_stride,
              src9, src10, src11, src12, src13, src14, src15, src16);
    src += src_stride_8x;
    src8 = LASX_LD(src);
    src17 = LASX_LD(src + 1);

    LASX_ILVL_H_8(src9, src0, src10, src1, src11, src2, src12, src3,
                  src13, src4, src14, src5, src15, src6, src16, src7,
                  src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_ILVL_H(src17, src8, src8);
    src0 = __lasx_xvhaddw_hu_bu(src0, src0);
    src1 = __lasx_xvhaddw_hu_bu(src1, src1);
    src2 = __lasx_xvhaddw_hu_bu(src2, src2);
    src3 = __lasx_xvhaddw_hu_bu(src3, src3);
    src4 = __lasx_xvhaddw_hu_bu(src4, src4);
    src5 = __lasx_xvhaddw_hu_bu(src5, src5);
    src6 = __lasx_xvhaddw_hu_bu(src6, src6);
    src7 = __lasx_xvhaddw_hu_bu(src7, src7);
    src8 = __lasx_xvhaddw_hu_bu(src8, src8);
    LASX_ADD_H_8(src0, src1, src1, src2, src2, src3, src3, src4,
                 src4, src5, src5, src6, src6, src7, src7, src8,
                 sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);
    sum0 = __lasx_xvaddi_hu(sum0, 1);
    sum1 = __lasx_xvaddi_hu(sum1, 1);
    sum2 = __lasx_xvaddi_hu(sum2, 1);
    sum3 = __lasx_xvaddi_hu(sum3, 1);
    sum4 = __lasx_xvaddi_hu(sum4, 1);
    sum5 = __lasx_xvaddi_hu(sum5, 1);
    sum6 = __lasx_xvaddi_hu(sum6, 1);
    sum7 = __lasx_xvaddi_hu(sum7, 1);
    sum0 = __lasx_xvsrai_h(sum0, 2);
    sum1 = __lasx_xvsrai_h(sum1, 2);
    sum2 = __lasx_xvsrai_h(sum2, 2);
    sum3 = __lasx_xvsrai_h(sum3, 2);
    sum4 = __lasx_xvsrai_h(sum4, 2);
    sum5 = __lasx_xvsrai_h(sum5, 2);
    sum6 = __lasx_xvsrai_h(sum6, 2);
    sum7 = __lasx_xvsrai_h(sum7, 2);
    LASX_PCKEV_B_4_128SV(sum1, sum0, sum3, sum2, sum5, sum4, sum7, sum6,
                         sum0, sum1, sum2, sum3);
    LASX_ST_D_2(sum0, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum0, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum1, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum1, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum2, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum2, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum3, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum3, 1, 3, dst, 8);
    dst += dst_stride;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_LD_8((src + 1), src_stride,
              src9, src10, src11, src12, src13, src14, src15, src16);
    src += src_stride_8x;
    src8 = LASX_LD(src);
    src17 = LASX_LD(src + 1);

    LASX_ILVL_H_8(src9, src0, src10, src1, src11, src2, src12, src3,
                  src13, src4, src14, src5, src15, src6, src16, src7,
                  src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_ILVL_H(src17, src8, src8);
    src0 = __lasx_xvhaddw_hu_bu(src0, src0);
    src1 = __lasx_xvhaddw_hu_bu(src1, src1);
    src2 = __lasx_xvhaddw_hu_bu(src2, src2);
    src3 = __lasx_xvhaddw_hu_bu(src3, src3);
    src4 = __lasx_xvhaddw_hu_bu(src4, src4);
    src5 = __lasx_xvhaddw_hu_bu(src5, src5);
    src6 = __lasx_xvhaddw_hu_bu(src6, src6);
    src7 = __lasx_xvhaddw_hu_bu(src7, src7);
    src8 = __lasx_xvhaddw_hu_bu(src8, src8);
    LASX_ADD_H_8(src0, src1, src1, src2, src2, src3, src3, src4,
                 src4, src5, src5, src6, src6, src7, src7, src8,
                 sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);
    sum0 = __lasx_xvaddi_hu(sum0, 1);
    sum1 = __lasx_xvaddi_hu(sum1, 1);
    sum2 = __lasx_xvaddi_hu(sum2, 1);
    sum3 = __lasx_xvaddi_hu(sum3, 1);
    sum4 = __lasx_xvaddi_hu(sum4, 1);
    sum5 = __lasx_xvaddi_hu(sum5, 1);
    sum6 = __lasx_xvaddi_hu(sum6, 1);
    sum7 = __lasx_xvaddi_hu(sum7, 1);
    sum0 = __lasx_xvsrai_h(sum0, 2);
    sum1 = __lasx_xvsrai_h(sum1, 2);
    sum2 = __lasx_xvsrai_h(sum2, 2);
    sum3 = __lasx_xvsrai_h(sum3, 2);
    sum4 = __lasx_xvsrai_h(sum4, 2);
    sum5 = __lasx_xvsrai_h(sum5, 2);
    sum6 = __lasx_xvsrai_h(sum6, 2);
    sum7 = __lasx_xvsrai_h(sum7, 2);
    LASX_PCKEV_B_4_128SV(sum1, sum0, sum3, sum2, sum5, sum4, sum7, sum6,
                         sum0, sum1, sum2, sum3);
    LASX_ST_D_2(sum0, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum0, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum1, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum1, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum2, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum2, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum3, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum3, 1, 3, dst, 8);
}

static void common_hv_bil_no_rnd_8x16_lasx(const uint8_t *src,
                                           int32_t src_stride,
                                           uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7, src8, src9;
    __m256i src10, src11, src12, src13, src14, src15, src16, src17;
    __m256i sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7;
    int32_t src_stride_8x = src_stride << 3;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_LD_8((src + 1), src_stride,
              src9, src10, src11, src12, src13, src14, src15, src16);
    src += src_stride_8x;
    src8 = LASX_LD(src);
    src17 = LASX_LD(src + 1);

    LASX_ILVL_H_8(src9, src0, src10, src1, src11, src2, src12, src3,
                  src13, src4, src14, src5, src15, src6, src16, src7,
                  src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_ILVL_H(src17, src8, src8);
    src0 = __lasx_xvhaddw_hu_bu(src0, src0);
    src1 = __lasx_xvhaddw_hu_bu(src1, src1);
    src2 = __lasx_xvhaddw_hu_bu(src2, src2);
    src3 = __lasx_xvhaddw_hu_bu(src3, src3);
    src4 = __lasx_xvhaddw_hu_bu(src4, src4);
    src5 = __lasx_xvhaddw_hu_bu(src5, src5);
    src6 = __lasx_xvhaddw_hu_bu(src6, src6);
    src7 = __lasx_xvhaddw_hu_bu(src7, src7);
    src8 = __lasx_xvhaddw_hu_bu(src8, src8);
    LASX_ADD_H_8(src0, src1, src1, src2, src2, src3, src3, src4,
                 src4, src5, src5, src6, src6, src7, src7, src8,
                 sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);
    sum0 = __lasx_xvaddi_hu(sum0, 1);
    sum1 = __lasx_xvaddi_hu(sum1, 1);
    sum2 = __lasx_xvaddi_hu(sum2, 1);
    sum3 = __lasx_xvaddi_hu(sum3, 1);
    sum4 = __lasx_xvaddi_hu(sum4, 1);
    sum5 = __lasx_xvaddi_hu(sum5, 1);
    sum6 = __lasx_xvaddi_hu(sum6, 1);
    sum7 = __lasx_xvaddi_hu(sum7, 1);
    sum0 = __lasx_xvsrai_h(sum0, 2);
    sum1 = __lasx_xvsrai_h(sum1, 2);
    sum2 = __lasx_xvsrai_h(sum2, 2);
    sum3 = __lasx_xvsrai_h(sum3, 2);
    sum4 = __lasx_xvsrai_h(sum4, 2);
    sum5 = __lasx_xvsrai_h(sum5, 2);
    sum6 = __lasx_xvsrai_h(sum6, 2);
    sum7 = __lasx_xvsrai_h(sum7, 2);
    LASX_PCKEV_B_4_128SV(sum1, sum0, sum3, sum2, sum5, sum4, sum7, sum6,
                         sum0, sum1, sum2, sum3);
    LASX_ST_D_2(sum0, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum0, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum1, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum1, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum2, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum2, 1, 3, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum3, 0, 2, dst, 8);
    dst += dst_stride;
    LASX_ST_D_2(sum3, 1, 3, dst, 8);
}

void ff_put_no_rnd_pixels16_xy2_8_lasx(uint8_t *block,
                                       const uint8_t *pixels,
                                       ptrdiff_t line_size, int h)
{
    if (h == 16) {
        common_hv_bil_no_rnd_16x16_lasx(pixels, line_size, block, line_size);
    } else if (h == 8) {
        common_hv_bil_no_rnd_8x16_lasx(pixels, line_size, block, line_size);
    }
}

static void common_hz_bil_no_rnd_8x8_lasx(const uint8_t *src, int32_t src_stride,
                                          uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    __m256i src8, src9, src10, src11, src12, src13, src14, src15;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_LD_8((src + 1), src_stride,
              src8, src9, src10, src11, src12, src13, src14, src15);
    LASX_PCKEV_D_4_128SV(src1, src0, src3, src2, src5, src4, src7, src6,
                         src0, src1, src2, src3);
    LASX_PCKEV_D_4_128SV(src9, src8, src11, src10, src13, src12, src15, src14,
                         src4, src5, src6, src7);
    LASX_PCKEV_Q_4(src1, src0, src3, src2, src5, src4, src7, src6,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src2);
    src1 = __lasx_xvavg_bu(src1, src3);
    LASX_ST_D_4(src0, 0, 1, 2, 3, dst, dst_stride);
    dst += (dst_stride << 2);
    LASX_ST_D_4(src1, 0, 1, 2, 3, dst, dst_stride);
}

static void common_hz_bil_no_rnd_4x8_lasx(const uint8_t *src, int32_t src_stride,
                                          uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src4, src5, src6, src7);
    LASX_PCKEV_D_4_128SV(src1, src0, src3, src2, src5, src4, src7, src6,
                         src0, src1, src2, src3);
    LASX_PCKEV_Q_2(src1, src0, src3, src2, src0, src1);
    src0 = __lasx_xvavg_bu(src0, src1);
    LASX_ST_D_4(src0, 0, 1, 2, 3, dst, dst_stride);
}

void ff_put_no_rnd_pixels8_x2_8_lasx(uint8_t *block, const uint8_t *pixels,
                                     ptrdiff_t line_size, int h)
{
    if (h == 8) {
        common_hz_bil_no_rnd_8x8_lasx(pixels, line_size, block, line_size);
    } else if (h == 4) {
        common_hz_bil_no_rnd_4x8_lasx(pixels, line_size, block, line_size);
    }
}

static void common_vt_bil_no_rnd_8x8_lasx(const uint8_t *src, int32_t src_stride,
                                          uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7, src8;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    src += (src_stride << 3);
    src8 = LASX_LD(src);

    LASX_PCKEV_D_4_128SV(src1, src0, src2, src1, src3, src2, src4, src3,
                         src0, src1, src2, src3);
    LASX_PCKEV_D_4_128SV(src5, src4, src6, src5, src7, src6, src8, src7,
                         src4, src5, src6, src7);
    LASX_PCKEV_Q_4(src2, src0, src3, src1, src6, src4, src7, src5,
                   src0, src1, src2, src3);
    src0 = __lasx_xvavg_bu(src0, src1);
    src1 = __lasx_xvavg_bu(src2, src3);
    LASX_ST_D_4(src0, 0, 1, 2, 3, dst, dst_stride);
    dst += (dst_stride << 2);
    LASX_ST_D_4(src1, 0, 1, 2, 3, dst, dst_stride);
}

static void common_vt_bil_no_rnd_4x8_lasx(const uint8_t *src, int32_t src_stride,
                                          uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    src += (src_stride << 2);
    src4 = LASX_LD(src);
    LASX_PCKEV_D_4_128SV(src1, src0, src2, src1, src3, src2, src4, src3,
                         src0, src1, src2, src3);
    LASX_PCKEV_Q_2(src2, src0, src3, src1, src0, src1);
    src0 = __lasx_xvavg_bu(src0, src1);
    LASX_ST_D_4(src0, 0, 1, 2, 3, dst, dst_stride);
}

void ff_put_no_rnd_pixels8_y2_8_lasx(uint8_t *block, const uint8_t *pixels,
                                     ptrdiff_t line_size, int h)
{
    if (h == 8) {
        common_vt_bil_no_rnd_8x8_lasx(pixels, line_size, block, line_size);
    } else if (h == 4) {
        common_vt_bil_no_rnd_4x8_lasx(pixels, line_size, block, line_size);
    }
}

static void common_hv_bil_no_rnd_8x8_lasx(const uint8_t *src, int32_t src_stride,
                                          uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    __m256i src8, src9, src10, src11, src12, src13, src14, src15, src16, src17;
    __m256i sum0, sum1, sum2, sum3;

    LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_LD_8((src + 1), src_stride,
              src9, src10, src11, src12, src13, src14, src15, src16);
    src += (src_stride << 3);
    src8 = LASX_LD(src);
    src17 = LASX_LD(src + 1);

    LASX_ILVL_B_8_128SV(src9, src0, src10, src1, src11, src2, src12, src3,
                        src13, src4, src14, src5, src15, src6, src16, src7,
                        src0, src1, src2, src3, src4, src5, src6, src7);
    LASX_ILVL_B_128SV(src17, src8, src8);
    LASX_PCKEV_Q_8(src1, src0, src2, src1, src3, src2, src4, src3,
                   src5, src4, src6, src5, src7, src6, src8, src7,
                   src0, src1, src2, src3, src4, src5, src6, src7);
    src0 = __lasx_xvhaddw_hu_bu(src0, src0);
    src1 = __lasx_xvhaddw_hu_bu(src1, src1);
    src2 = __lasx_xvhaddw_hu_bu(src2, src2);
    src3 = __lasx_xvhaddw_hu_bu(src3, src3);
    src4 = __lasx_xvhaddw_hu_bu(src4, src4);
    src5 = __lasx_xvhaddw_hu_bu(src5, src5);
    src6 = __lasx_xvhaddw_hu_bu(src6, src6);
    src7 = __lasx_xvhaddw_hu_bu(src7, src7);
    LASX_ADD_H_4(src0, src1, src2, src3, src4, src5, src6, src7,
                 sum0, sum1, sum2, sum3);
    sum0 = __lasx_xvaddi_hu(sum0, 1);
    sum1 = __lasx_xvaddi_hu(sum1, 1);
    sum2 = __lasx_xvaddi_hu(sum2, 1);
    sum3 = __lasx_xvaddi_hu(sum3, 1);
    sum0 = __lasx_xvsrai_h(sum0, 2);
    sum1 = __lasx_xvsrai_h(sum1, 2);
    sum2 = __lasx_xvsrai_h(sum2, 2);
    sum3 = __lasx_xvsrai_h(sum3, 2);
    LASX_PCKEV_B_2_128SV(sum1, sum0, sum3, sum2, sum0, sum1);
    LASX_ST_D_4(sum0, 0, 2, 1, 3, dst, dst_stride);
    dst += (dst_stride << 2);
    LASX_ST_D_4(sum1, 0, 2, 1, 3, dst, dst_stride);
}

static void common_hv_bil_no_rnd_4x8_lasx(const uint8_t *src, int32_t src_stride,
                                          uint8_t *dst, int32_t dst_stride)
{
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    __m256i src8, src9, sum0, sum1;

    LASX_LD_4(src, src_stride, src0, src1, src2, src3);
    LASX_LD_4((src + 1), src_stride, src5, src6, src7, src8);
    src += (src_stride << 2);
    src4 = LASX_LD(src);
    src9 = LASX_LD(src + 1);

    LASX_ILVL_B_4_128SV(src5, src0, src6, src1, src7, src2, src8, src3,
                        src0, src1, src2, src3);
    LASX_ILVL_B_128SV(src9, src4, src4);
    LASX_PCKEV_Q_4(src1, src0, src2, src1, src3, src2, src4, src3,
                   src0, src1, src2, src3);
    src0 = __lasx_xvhaddw_hu_bu(src0, src0);
    src1 = __lasx_xvhaddw_hu_bu(src1, src1);
    src2 = __lasx_xvhaddw_hu_bu(src2, src2);
    src3 = __lasx_xvhaddw_hu_bu(src3, src3);
    LASX_ADD_H_2(src0, src1, src2, src3, sum0, sum1);
    sum0 = __lasx_xvaddi_hu(sum0, 1);
    sum1 = __lasx_xvaddi_hu(sum1, 1);
    sum0 = __lasx_xvsrai_h(sum0, 2);
    sum1 = __lasx_xvsrai_h(sum1, 2);
    LASX_PCKEV_B_128SV(sum1, sum0, sum0);
    LASX_ST_D_4(sum0, 0, 2, 1, 3, dst, dst_stride);
}

void ff_put_no_rnd_pixels8_xy2_8_lasx(uint8_t *block, const uint8_t *pixels,
                                      ptrdiff_t line_size, int h)
{
    if (h == 8) {
        common_hv_bil_no_rnd_8x8_lasx(pixels, line_size, block, line_size);
    } else if (h == 4) {
        common_hv_bil_no_rnd_4x8_lasx(pixels, line_size, block, line_size);
    }
}

static void common_hv_bil_16w_lasx(const uint8_t *src, int32_t src_stride,
                                   uint8_t *dst, int32_t dst_stride,
                                   uint8_t height)
{
    uint8_t loop_cnt;
    __m256i src0, src1, src2, src3, src4, src5, src6, src7, src8, src9;
    __m256i src10, src11, src12, src13, src14, src15, src16, src17;
    __m256i sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7;
    int32_t src_stride_8x = (src_stride << 3);

    for (loop_cnt = (height >> 3); loop_cnt--;) {
        LASX_LD_8(src, src_stride, src0, src1, src2, src3, src4, src5, src6, src7);
        LASX_LD_8((src + 1), src_stride,
                  src9, src10, src11, src12, src13, src14, src15, src16);
        src += src_stride_8x;

        src8 = LASX_LD(src);
        src17 = LASX_LD(src + 1);

        LASX_ILVL_H_8(src9, src0, src10, src1, src11, src2, src12, src3,
                      src13, src4, src14, src5, src15, src6, src16, src7,
                      src0, src1, src2, src3, src4, src5, src6, src7);
        LASX_ILVL_H(src17, src8, src8);
        src0 = __lasx_xvhaddw_hu_bu(src0, src0);
        src1 = __lasx_xvhaddw_hu_bu(src1, src1);
        src2 = __lasx_xvhaddw_hu_bu(src2, src2);
        src3 = __lasx_xvhaddw_hu_bu(src3, src3);
        src4 = __lasx_xvhaddw_hu_bu(src4, src4);
        src5 = __lasx_xvhaddw_hu_bu(src5, src5);
        src6 = __lasx_xvhaddw_hu_bu(src6, src6);
        src7 = __lasx_xvhaddw_hu_bu(src7, src7);
        src8 = __lasx_xvhaddw_hu_bu(src8, src8);
        LASX_ADD_H_8(src0, src1, src1, src2, src2, src3, src3, src4,
                     src4, src5, src5, src6, src6, src7, src7, src8,
                     sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7);
        LASX_SRARI_H_4(sum0, sum1, sum2, sum3, sum0, sum1, sum2, sum3, 2);
        LASX_SRARI_H_4(sum4, sum5, sum6, sum7, sum4, sum5, sum6, sum7, 2);
        LASX_PCKEV_B_4_128SV(sum1, sum0, sum3, sum2, sum5, sum4, sum7, sum6,
                             sum0, sum1, sum2, sum3);
        LASX_ST_D_2(sum0, 0, 2, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum0, 1, 3, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum1, 0, 2, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum1, 1, 3, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum2, 0, 2, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum2, 1, 3, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum3, 0, 2, dst, 8);
        dst += dst_stride;
        LASX_ST_D_2(sum3, 1, 3, dst, 8);
        dst += dst_stride;
    }
}

void ff_put_pixels16_xy2_8_lasx(uint8_t *block, const uint8_t *pixels,
                                ptrdiff_t line_size, int h)
{
    common_hv_bil_16w_lasx(pixels, line_size, block, line_size, h);
}

static void common_hv_bil_8w_lasx(const uint8_t *src, int32_t src_stride,
                                  uint8_t *dst, int32_t dst_stride,
                                  uint8_t height)
{
    uint8_t loop_cnt;
    __m256i src0, src1, src2, src3, src4, src5, src6, src7;
    __m256i src8, src9, sum0, sum1;
    int32_t src_stride_4x = (src_stride << 2);
    int32_t dst_stride_4x = (dst_stride << 2);

    src0 = LASX_LD(src);
    src5 = LASX_LD(src + 1);
    src += src_stride;

    for (loop_cnt = (height >> 2); loop_cnt--;) {
        LASX_LD_4(src, src_stride, src1, src2, src3, src4);
        LASX_LD_4((src + 1), src_stride, src6, src7, src8, src9);
        src += src_stride_4x;
        LASX_ILVL_B_4_128SV(src5, src0, src6, src1, src7, src2, src8, src3,
                            src0, src1, src2, src3);
        LASX_ILVL_B_128SV(src9, src4, src5);
        LASX_PCKEV_Q_4(src1, src0, src2, src1, src3, src2, src5, src3,
                       src0, src1, src2, src3);
        src0 = __lasx_xvhaddw_hu_bu(src0, src0);
        src1 = __lasx_xvhaddw_hu_bu(src1, src1);
        src2 = __lasx_xvhaddw_hu_bu(src2, src2);
        src3 = __lasx_xvhaddw_hu_bu(src3, src3);
        LASX_ADD_H_2(src0, src1, src2, src3, sum0, sum1);
        LASX_SRARI_H_2(sum0, sum1, sum0, sum1, 2);
        LASX_PCKEV_B_128SV(sum1, sum0, sum0);
        LASX_ST_D_4(sum0, 0, 2, 1, 3, dst, dst_stride);
        dst += dst_stride_4x;
        src0 = src4;
        src5 = src9;
    }
}

void ff_put_pixels8_xy2_8_lasx(uint8_t *block, const uint8_t *pixels,
                               ptrdiff_t line_size, int h)
{
    common_hv_bil_8w_lasx(pixels, line_size, block, line_size, h);
}
