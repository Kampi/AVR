/*
 * Font.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *	Website: www.kampis-elektroecke.de
 *  File info: Font set for displays.

  GNU GENERAL PUBLIC LICENSE:
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file Common/Font/Font.c
 *  @brief Common font set for displays.
 *
 *  @author Daniel Kampert
 */
#include "Common/Font/Font.h"

const uint8_t* FontTable[95] = {
	__Char_32, __Char_33, __Char_34, __Char_35, __Char_36, __Char_37, __Char_38, __Char_39,
	__Char_40, __Char_41, __Char_42, __Char_43, __Char_44, __Char_45, __Char_46, __Char_47,
	__Char_N0, __Char_N1, __Char_N2, __Char_N3, __Char_N4, __Char_N5, __Char_N6, __Char_N7,
	__Char_N8, __Char_N9, __Char_58, __Char_59, __Char_60, __Char_61, __Char_62, __Char_63,
	__Char_64, __Char_UA, __Char_UB, __Char_UC,	__Char_UD, __Char_UE, __Char_UF, __Char_UG,
	__Char_UH, __Char_UI, __Char_UJ, __Char_UK, __Char_UL, __Char_UM, __Char_UN, __Char_UO,
	__Char_UP, __Char_UQ, __Char_UR, __Char_US, __Char_UT, __Char_UU, __Char_UV, __Char_UW,
	__Char_UX, __Char_UY, __Char_UZ, __Char_91, __Char_92, __Char_93, __Char_94, __Char_95,
	__Char_96, __Char_la, __Char_lb, __Char_lc, __Char_ld, __Char_le, __Char_lf, __Char_lg,
	__Char_lh, __Char_li, __Char_lj, __Char_lk, __Char_ll, __Char_lm, __Char_ln, __Char_lo,
	__Char_lp, __Char_lq, __Char_lr, __Char_ls,	__Char_lt, __Char_lu, __Char_lv, __Char_lw,
	__Char_lx, __Char_ly, __Char_lz, __Char_123, __Char_124, __Char_125, __Char_126
};

// 0x20 - 32 - ' '
const uint8_t __Char_32[3] = {
	2, 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 0, 0)
};

// 0x21 - 33 - '!'
const uint8_t __Char_33[2] = {
	1, 
	Bits2Bytes(1, 0, 1, 1, 1, 1, 1, 0)
};

// 0x22 - 34 - '"'
const uint8_t __Char_34[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0)
};

// 0x23 - 35 - '#'
const uint8_t __Char_35[6] = {
	5, 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0)
};

// 0x24 - 36 - '$'
const uint8_t __Char_36[6] = {
	5, 
	Bits2Bytes(0, 1, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 1, 0, 1, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 1, 0, 1, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 1, 0, 0)
};

// 0x25 - 37 - '%'
const uint8_t __Char_37[6] = {
	5, 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 1, 0, 0)
};

// 0x26 - 38 - '&'
const uint8_t __Char_38[6] = {
	5, 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 1, 0, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 0, 0)
};

// 0x27 - 39 - '''
const uint8_t __Char_39[2] = {
	1, 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0)
};

// 0x28 - 40 - '('
const uint8_t __Char_40[4] = {
	3, 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x29 - 41 - ')'
const uint8_t __Char_41[4] = {
	3, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0)
};

// 0x2A - 42 - '*'
const uint8_t __Char_42[4] = {
	3, 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0)
};

// 0x2B - 43 - '+'
const uint8_t __Char_43[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0)
};

// 0x2C - 44 - ', '
const uint8_t __Char_44[2] = {
	1, 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0)
};

// 0x2D - 45 - '-'
const uint8_t __Char_45[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0)
};

// 0x2E - 46 - '.'
const uint8_t __Char_46[2] = {
	1, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0)
};

// 0x2F - 47 - '/'
const uint8_t __Char_47[4] = {
	3, 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0)
};

// 0x30 - 48 - '0'
const uint8_t __Char_N0[6] = {
	5, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0)
};

// 0x31 - 49 - '1'
const uint8_t __Char_N1[6] = {
	3, 
	Bits2Bytes(1, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0)
};

// 0x32 - 50 - '2'
const uint8_t __Char_N2[6] = {
	5, 
	Bits2Bytes(1, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 1, 0, 0)
};

// 0x33 - 51 - '3'
const uint8_t __Char_N3[6] = {
	5, 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0)
};

// 0x34 - 52 - '4'
const uint8_t __Char_N4[6] = {
	5, 
	Bits2Bytes(0, 0, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 0, 0, 0)
};

// 0x35 - 53 - '5'
const uint8_t __Char_N5[6] = {
	5, 
	Bits2Bytes(0, 1, 0, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 1, 0)
};

// 0x36 - 54 - '6'
const uint8_t __Char_N6[6] = {
	5, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 0, 0, 1, 0, 0)
};

// 0x37 - 55 - '7'
const uint8_t __Char_N7[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0)
};

// 0x38 - 56 - '8'
const uint8_t __Char_N8[6] = {
	5, 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0)
};

// 0x39 - 57 - '9'
const uint8_t __Char_N9[6] = {
	5, 
	Bits2Bytes(0, 1, 0, 0, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0)
};

// 0x3A - 58 - ':'
const uint8_t __Char_58[2] = {
	1, 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0)
};

// 0x3B - 59 - ';'
const uint8_t __Char_59[2] = {
	1, 
	Bits2Bytes(1, 1, 0, 0, 0, 1, 0, 0)
};

// 0x3C - 60 - '<'
const uint8_t __Char_60[5] = {
	4, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x3D - 61 - '='
const uint8_t __Char_61[5] = {
	4, 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0)
};

// 0x3E - 62 - '>'
const uint8_t __Char_62[5] = {
	4, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0)
};

// 0x3F - 63 - '?'
const uint8_t __Char_63[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 1, 0, 0)
};

// 0x40 - 64 - '@'
const uint8_t __Char_64[9] = {
	8, 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 1, 1, 0, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 1, 0, 0, 0)
};

// 0x41 - 65 - 'A'
const uint8_t __Char_UA[6] = {
	5, 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 1, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0)
};

// 0x42 - 66 - 'B'
const uint8_t __Char_UB[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0)
};

// 0x43 - 67 - 'C'
const uint8_t __Char_UC[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0)
};

// 0x44 - 68 - 'D'
const uint8_t __Char_UD[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0)
};

// 0x45 - 69 - 'E'
const uint8_t __Char_UE[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x46 - 70 - 'F'
const uint8_t __Char_UF[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0)
	};

// 0x47 - 71 - 'G'
const uint8_t __Char_UG[6] = {
	5, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 1, 0, 0)
};

// 0x48 - 72 - 'H'
const uint8_t __Char_UH[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)
};

// 0x49 - 73 - 'I'
const uint8_t __Char_UI[4] = {
	3, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x4A - 74 - 'J'
const uint8_t __Char_UJ[5] = {
	4, 
	Bits2Bytes(0, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 1, 0)
};

// 0x4B - 75 - 'K'
const uint8_t __Char_UK[6] = {
	5, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x4C - 76 - 'L'
const uint8_t __Char_UL[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0)
};

// 0x4D - 77 - 'M'
const uint8_t __Char_UM[6] = {
	5, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)
};

// 0x4E - 78 - 'N'
const uint8_t __Char_UN[6] = {
	5, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)
};

// 0x4F - 79 - 'O'
const uint8_t __Char_UO[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0)
};

// 0x50 - 80 - 'P'
const uint8_t __Char_UP[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 1, 0, 0)
};

// 0x51 - 81 - 'Q'
const uint8_t __Char_UQ[6] = {
	5, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 1, 1, 1, 0, 0)
};

// 0x52 - 82 - 'R'
const uint8_t __Char_UR[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 1, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 1, 0, 0, 1, 1, 0, 0)
};

// 0x53 - 83 - 'S'
const uint8_t __Char_US[5] = {
	4, 
	Bits2Bytes(0, 1, 0, 0, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 0, 0, 1, 0, 0)
};

// 0x54 - 84 - 'T'
const uint8_t __Char_UT[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0)
};

// 0x55 - 85 - 'U'
const uint8_t __Char_UU[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 1, 0)
};

// 0x56 - 86 - 'V'
const uint8_t __Char_UV[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 0, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 1, 1, 0)
};

// 0x57 - 87 - 'W'
const uint8_t __Char_UW[6] = {
	5, 
	Bits2Bytes(0, 0, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 1, 1, 0)
};

// 0x58 - 88 - 'X'
const uint8_t __Char_UX[6] = {
	5, 
	Bits2Bytes(1, 1, 0, 0, 0, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 1, 1, 0)
};

// 0x59 - 89 - 'Y'
const uint8_t __Char_UY[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0)
};

// 0x5A - 90 - 'Z'
const uint8_t __Char_UZ[6] = {
	5, 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 1, 1, 0)
};

// 0x5B - 91 - '['
const uint8_t __Char_91[4] = {
	3, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x5C - 92 - '\'
const uint8_t __Char_92[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0)
};

// 0x5D - 93 - ']'
const uint8_t __Char_93[4] = {3, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)};

// 0x5E - 94 - '^'
const uint8_t __Char_94[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0)
};

// 0x5F - 95 - '_'
const uint8_t __Char_95[4] = {
	3, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0)
};

// 0x60 - 96 - '`'
const uint8_t __Char_96[3] = {
	2, 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 1, 0, 0)
};

// 0x61 - 97 - 'a'
const uint8_t __Char_la[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0)
};

// 0x62 - 98 - 'b'
const uint8_t __Char_lb[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0)
};

// 0x63 - 99 - 'c'
const uint8_t __Char_lc[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0)
};

// 0x64 - 100 - 'd'
const uint8_t __Char_ld[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)
};

// 0x65 - 101 - 'e'
const uint8_t __Char_le[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 0, 0, 0, 0)
};

// 0x66 - 102 - 'f'
const uint8_t __Char_lf[4] = {
	3, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 1, 0), 
	Bits2Bytes(0, 0, 0, 0, 0, 0, 1, 0)
};

// 0x67 - 103 - 'g'
const uint8_t __Char_lg[5] = {
	4, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 0, 0, 0)
};

// 0x68 - 104 - 'h'
const uint8_t __Char_lh[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 0, 0, 0)
};

// 0x69 - 105 - 'i'
const uint8_t __Char_li[2] = {
	1, 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 1, 0)
};

// 0x6A - 106 - 'j'
const uint8_t __Char_lj[3] = {
	2, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 0, 1, 0)
};

// 0x6B - 107 - 'k'
const uint8_t __Char_lk[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0)
};

// 0x6C - 108 - 'l'
const uint8_t __Char_ll[2] = {
	1, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)
};

// 0x6D - 109 - 'm'
const uint8_t __Char_lm[6] = {
	5, 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 0, 0, 0)
};

// 0x6E - 110 - 'n'
const uint8_t __Char_ln[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 0, 0, 0, 0)
};

// 0x6F - 111 - 'o'
const uint8_t __Char_lo[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 0, 0, 0, 0)
};

// 0x70 - 112 - 'p'
const uint8_t __Char_lp[5] = {
	4, 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0)
};

// 0x71 - 113 - 'q'
const uint8_t __Char_lq[5] = {
	4, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0)
};

// 0x72 - 114 - 'r'
const uint8_t __Char_lr[4] = {
	3, 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0)
};

// 0x73 - 115 - 's'
const uint8_t __Char_ls[5] = {
	4, 
	Bits2Bytes(1, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 0, 1, 0, 0, 0)
};

// 0x74 - 116 - 't'
const uint8_t __Char_lt[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0)
};

// 0x75 - 117 - 'u'
const uint8_t __Char_lu[5] = {
	4, 
	Bits2Bytes(0, 1, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 1, 1, 1, 1, 0, 0, 0)
};

// 0x76 - 118 - 'v'
const uint8_t __Char_lv[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 1, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 1, 0, 0, 0)
};

// 0x77 - 119 - 'w'
const uint8_t __Char_lw[6] = {
	5, 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 1, 0, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 1, 1, 0, 0, 0)
};

// 0x78 - 120 - 'x'
const uint8_t __Char_lx[6] = {
	5, 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 1, 0, 0, 0)
};

// 0x79 - 121 - 'y'
const uint8_t __Char_ly[5] = {
	4, 
	Bits2Bytes(0, 0, 0, 1, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 1, 1, 0, 0, 0)
};

// 0x7A - 122 - 'z'
const uint8_t __Char_lz[4] = {
	3, 
	Bits2Bytes(1, 1, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 1, 0, 1, 0, 0, 0), 
	Bits2Bytes(1, 0, 0, 1, 1, 0, 0, 0)
};

// 0x7B - 123 - '{'
const uint8_t __Char_123[4] = {
	3, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0), 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0)
};

// 0x7C - 124 - '|'
const uint8_t __Char_124[2] = {
	1, 
	Bits2Bytes(1, 1, 1, 1, 1, 1, 1, 0)
};

// 0x7D - 125 - '}'
const uint8_t __Char_125[4] = {
	3, 
	Bits2Bytes(1, 0, 0, 0, 0, 0, 1, 0), 
	Bits2Bytes(0, 1, 1, 0, 1, 1, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0)
};

// 0x7E - 126 - '~'
const uint8_t __Char_126[6] = {
	5, 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 0, 1, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 1, 0, 0, 0, 0, 0), 
	Bits2Bytes(0, 0, 0, 1, 0, 0, 0, 0)
};