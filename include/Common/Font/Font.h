/*
 * Font.h
 *
 *  Copyright (C) Daniel Kampert, 2018
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

/** @file Common/Font/Font.h
 *  @brief Font set for displays.
 *
 *  @author Daniel Kampert
 */

#ifndef FONT_H_
#define FONT_H_

 #include <stdint.h>

 #define FONT_HEIGHT			8						/**< Font height in pixel */

 #define Bits2Bytes(B7, B6, B5, B4, B3, B2, B1, B0)		((uint8_t)((B7 << 0x07) | (B6 << 0x06) | (B5 << 0x05) | (B4 << 0x04) | \
																   (B3 << 0x03) | (B2 << 0x02) | (B1 << 0x01) | (B0 << 0x00)))

 /** @brief	Character table
  */
 extern const uint8_t* FontTable[95];

 /** 
  * @brief	Character data
  *			Each character is defined by width (first element) and a byte pattern (all other elements)
  */
 extern const uint8_t __Char_32[3];
 extern const uint8_t __Char_33[2];
 extern const uint8_t __Char_34[4];
 extern const uint8_t __Char_35[6];
 extern const uint8_t __Char_36[6];
 extern const uint8_t __Char_37[6];
 extern const uint8_t __Char_38[6];
 extern const uint8_t __Char_39[2];
 extern const uint8_t __Char_40[4];
 extern const uint8_t __Char_41[4];
 extern const uint8_t __Char_42[4];
 extern const uint8_t __Char_43[4];
 extern const uint8_t __Char_44[2];
 extern const uint8_t __Char_45[4];
 extern const uint8_t __Char_46[2];
 extern const uint8_t __Char_47[4];
 extern const uint8_t __Char_N0[6];
 extern const uint8_t __Char_N1[6];
 extern const uint8_t __Char_N2[6];
 extern const uint8_t __Char_N3[6];
 extern const uint8_t __Char_N4[6];
 extern const uint8_t __Char_N5[6];
 extern const uint8_t __Char_N6[6];
 extern const uint8_t __Char_N7[6];
 extern const uint8_t __Char_N8[6];
 extern const uint8_t __Char_N9[6];
 extern const uint8_t __Char_58[2];
 extern const uint8_t __Char_59[2];
 extern const uint8_t __Char_60[5];
 extern const uint8_t __Char_61[5];
 extern const uint8_t __Char_62[5];
 extern const uint8_t __Char_63[6];
 extern const uint8_t __Char_64[9];
 extern const uint8_t __Char_UA[6];
 extern const uint8_t __Char_UB[5];
 extern const uint8_t __Char_UC[5];
 extern const uint8_t __Char_UD[5];
 extern const uint8_t __Char_UE[5];
 extern const uint8_t __Char_UF[5];
 extern const uint8_t __Char_UG[6];
 extern const uint8_t __Char_UH[5];
 extern const uint8_t __Char_UI[4];
 extern const uint8_t __Char_UJ[5];
 extern const uint8_t __Char_UK[6];
 extern const uint8_t __Char_UL[5];
 extern const uint8_t __Char_UM[6];
 extern const uint8_t __Char_UN[6];
 extern const uint8_t __Char_UO[5];
 extern const uint8_t __Char_UP[5];
 extern const uint8_t __Char_UQ[6];
 extern const uint8_t __Char_UR[5];
 extern const uint8_t __Char_US[5];
 extern const uint8_t __Char_UT[6];
 extern const uint8_t __Char_UU[5];
 extern const uint8_t __Char_UV[6];
 extern const uint8_t __Char_UW[6];
 extern const uint8_t __Char_UX[6];
 extern const uint8_t __Char_UY[6];
 extern const uint8_t __Char_UZ[6];
 extern const uint8_t __Char_91[4];
 extern const uint8_t __Char_92[4];
 extern const uint8_t __Char_93[4];
 extern const uint8_t __Char_94[4];
 extern const uint8_t __Char_95[4];
 extern const uint8_t __Char_96[3];
 extern const uint8_t __Char_la[5];
 extern const uint8_t __Char_lb[5];
 extern const uint8_t __Char_lc[5];
 extern const uint8_t __Char_ld[5];
 extern const uint8_t __Char_le[5];
 extern const uint8_t __Char_lf[4];
 extern const uint8_t __Char_lg[5];
 extern const uint8_t __Char_lh[5];
 extern const uint8_t __Char_li[2];
 extern const uint8_t __Char_lj[3];
 extern const uint8_t __Char_lk[5];
 extern const uint8_t __Char_ll[2];
 extern const uint8_t __Char_lm[6];
 extern const uint8_t __Char_ln[5];
 extern const uint8_t __Char_lo[5];
 extern const uint8_t __Char_lp[5];
 extern const uint8_t __Char_lq[5];
 extern const uint8_t __Char_lr[4];
 extern const uint8_t __Char_ls[5];
 extern const uint8_t __Char_lt[4];
 extern const uint8_t __Char_lu[5];
 extern const uint8_t __Char_lv[6];
 extern const uint8_t __Char_lw[6];
 extern const uint8_t __Char_lx[6];
 extern const uint8_t __Char_ly[5];
 extern const uint8_t __Char_lz[4];
 extern const uint8_t __Char_123[4];
 extern const uint8_t __Char_124[2];
 extern const uint8_t __Char_125[4];
 extern const uint8_t __Char_126[6];

#endif /* FONT_H_ */