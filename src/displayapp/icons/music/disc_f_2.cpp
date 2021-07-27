/*  Copyright (C) 2020 Avamander

    This file is part of InfiniTime.

    InfiniTime is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    InfiniTime is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_DISC_F_2
#define LV_ATTRIBUTE_IMG_DISC_F_2
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_DISC_F_2 uint8_t disc_f_2_map[] = {
    0xbd, 0xc1, 0xbe, 0xff,  /* Color of index 0: foreground */
    0x00, 0x00, 0x00, 0x00,  /* Color of index 1: background */
    
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0,
    0xff, 0xff, 0xfc, 0x00,
    0xff, 0xff, 0xf0, 0x0f,
    0xff, 0xff, 0xc0, 0xff,
    0xff, 0xff, 0x07, 0xff,
    0xff, 0xfc, 0x1f, 0xff,
    0xff, 0xf8, 0x7f, 0xf1,
    0xff, 0xf0, 0xff, 0x00,
    0xff, 0xe3, 0xfc, 0x03,
    0xff, 0xc7, 0xf0, 0x3f,
    0xff, 0x8f, 0xf0, 0xff,
    0xff, 0x1f, 0xf3, 0xff,
    0xfe, 0x3f, 0xff, 0xff,
    0xfc, 0x7f, 0xff, 0xff,
    0xfc, 0x7f, 0xff, 0xff,
    0xf8, 0xff, 0xff, 0xff,
    0xf9, 0xff, 0xff, 0xff,
    0xf1, 0xff, 0xff, 0xff,
    0xf3, 0xff, 0xff, 0xff,
    0xe3, 0xff, 0xff, 0xff,
    0xe7, 0xff, 0xff, 0xff,
    0xc7, 0xff, 0xff, 0xff,
    0xc7, 0xff, 0xff, 0xff,
    0xcf, 0xff, 0xff, 0xff,
    0xcf, 0xff, 0xff, 0xff,
    0x8f, 0xff, 0xff, 0xff,
    0x8f, 0xff, 0xff, 0xf8,
    0x9f, 0xff, 0xff, 0xf0,
    0x9f, 0xff, 0xff, 0xe3,
    0x9f, 0xff, 0xff, 0xe7,
    0x9f, 0xff, 0xff, 0xe7,
};

const lv_img_dsc_t disc_f_2 = {
    {
        LV_IMG_CF_INDEXED_1BIT,
        0,
        0,
        32,
        32
    },
    136,
    disc_f_2_map
};

