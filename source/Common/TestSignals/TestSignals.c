/*
 * TestSignals.c
 *
 *  Copyright (C) Daniel Kampert, 2020
 *  Website: www.kampis-elektroecke.de
 *  File info: Test patterns for analog signals.

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

/** @file Common/TestSignals/TestSignals.c
 *  @brief Test patterns for analog signals.
 *
 *  @author Daniel Kampert
 */

#include "Common/TestSignals/TestSignals.h"

uint16_t Sine_120Hz[TESTSIGNAL_LENGTH] = {
		0x0000,	0x0203,	0x0405,	0x0608,	0x080A,	0x0A0B,	0x0C0C,	0x0E0C,
		0x100B,	0x1209,	0x1406,	0x1602,	0x17FC,	0x19F5,	0x1BEC,	0x1DE2,
		0x1FD5,	0x21C7,	0x23B6,	0x25A3,	0x278E,	0x2976,	0x2B5C,	0x2D3F,
		0x2F1F,	0x30FC,	0x32D6,	0x34AD,	0x3680,	0x3850,	0x3A1C,	0x3BE5,
		0x3DAA,	0x3F6B,	0x4128,	0x42E1,	0x4496,	0x4646,	0x47F2,	0x499A,
		0x4B3D,	0x4CDB,	0x4E74,	0x5008,	0x5197,	0x5321,	0x54A6,	0x5625,
		0x579F,	0x5914,	0x5A82,	0x5BEC,	0x5D4F,	0x5EAC,	0x6004,	0x6155,
		0x62A0,	0x63E5,	0x6524,	0x665C,	0x678E,	0x68B9,	0x69DE,	0x6AFC,
		0x6C13,	0x6D23,	0x6E2D,	0x6F2F,	0x702B,	0x711F,	0x720D,	0x72F3,
		0x73D1,	0x74A9,	0x7579, 0x7642,	0x7703,	0x77BD,	0x786F,	0x7919,
		0x79BC,	0x7A57,	0x7AEB,	0x7B77,	0x7BFB,	0x7C77,	0x7CEB,	0x7D57,
		0x7DBC, 0x7E18,	0x7E6D,	0x7EB9,	0x7EFE,	0x7F3A,	0x7F6F,	0x7F9B,
		0x7FBF,	0x7FDC,	0x7FF0,	0x7FFC,	0x7FFF,	0x7FFC,	0x7FF0,	0x7FDC,
		0x7FBF,	0x7F9B,	0x7F6F,	0x7F3A,	0x7EFE,	0x7EB9,	0x7E6D,	0x7E18,
		0x7DBC,	0x7D57,	0x7CEB,	0x7C77,	0x7BFB,	0x7B77,	0x7AEB,	0x7A57,
		0x79BC,	0x7919,	0x786F,	0x77BD,	0x7703,	0x7642,	0x7579,	0x74A9,
		0x73D1,	0x72F3,	0x720D,	0x711F,	0x702B,	0x6F2F,	0x6E2D,	0x6D23,
		0x6C13,	0x6AFC,	0x69DE,	0x68B9,	0x678E,	0x665C,	0x6524,	0x63E5,
		0x62A0,	0x6155,	0x6004,	0x5EAC,	0x5D4F,	0x5BEC,	0x5A82,	0x5914,
		0x579F,	0x5625,	0x54A6,	0x5321,	0x5197,	0x5008,	0x4E74,	0x4CDB,
		0x4B3D,	0x499A, 0x47F2,	0x4646,	0x4496,	0x42E1,	0x4128,	0x3F6B,
		0x3DAA,	0x3BE5,	0x3A1C,	0x3850,	0x3680,	0x34AD,	0x32D6,	0x30FC,
		0x2F1F,	0x2D3F,	0x2B5C,	0x2976,	0x278E,	0x25A3,	0x23B6,	0x21C7,
		0x1FD5,	0x1DE2,	0x1BEC,	0x19F5,	0x17FC,	0x1602,	0x1406,	0x1209,
		0x100B,	0x0E0C,	0x0C0C,	0x0A0B,	0x080A,	0x0608,	0x0405,	0x0203,
		0x0000,	0xFDFD,	0xFBFB, 0xF9F8,	0xF7F6,	0xF5F5,	0xF3F4,	0xF1F4,
		0xEFF5,	0xEDF7,	0xEBFA,	0xE9FE,	0xE804,	0xE60B,	0xE414,	0xE21E,
		0xE02B,	0xDE39,	0xDC4A,	0xDA5D,	0xD872,	0xD68A,	0xD4A4,	0xD2C1,
		0xD0E1,	0xCF04,	0xCD2A,	0xCB53,	0xC980,	0xC7B0,	0xC5E4,	0xC41B,
		0xC256,	0xC095,	0xBED8,	0xBD1F,	0xBB6A,	0xB9BA,	0xB80E,	0xB666,
		0xB4C3,	0xB325,	0xB18C,	0xAFF8,	0xAE69, 0xACDF,	0xAB5A,	0xA9DB,
		0xA861,	0xA6EC,	0xA57E,	0xA414,	0xA2B1,	0xA154,	0x9FFC,	0x9EAB,
		0x9D60,	0x9C1B,	0x9ADC,	0x99A4,	0x9872,	0x9747,	0x9622,	0x9504,
		0x93ED,	0x92DD,	0x91D3,	0x90D1,	0x8FD5,	0x8EE1,	0x8DF3,	0x8D0D,
		0x8C2F,	0x8B57,	0x8A87,	0x89BE,	0x88FD,	0x8843,	0x8791,	0x86E7,
		0x8644,	0x85A9,	0x8515, 0x8489,	0x8405,	0x8389,	0x8315,	0x82A9,
		0x8244,	0x81E8,	0x8193, 0x8147,	0x8102,	0x80C6,	0x8091,	0x8065,
		0x8041,	0x8024,	0x8010, 0x8004,	0x8000,	0x8004,	0x8010,	0x8024,
		0x8041,	0x8065,	0x8091, 0x80C6,	0x8102,	0x8147,	0x8193,	0x81E8,
		0x8244,	0x82A9,	0x8315,	0x8389,	0x8405,	0x8489,	0x8515,	0x85A9,
		0x8644,	0x86E7,	0x8791,	0x8843,	0x88FD,	0x89BE,	0x8A87,	0x8B57,
		0x8C2F,	0x8D0D,	0x8DF3,	0x8EE1,	0x8FD5,	0x90D1,	0x91D3,	0x92DD,
		0x93ED,	0x9504,	0x9622,	0x9747,	0x9872,	0x99A4,	0x9ADC,	0x9C1B,
		0x9D60,	0x9EAB,	0x9FFC,	0xA154,	0xA2B1,	0xA414,	0xA57E,	0xA6EC,
		0xA861,	0xA9DB,	0xAB5A,	0xACDF,	0xAE69,	0xAFF8,	0xB18C,	0xB325,
		0xB4C3,	0xB666,	0xB80E,	0xB9BA,	0xBB6A,	0xBD1F,	0xBED8,	0xC095,
		0xC256,	0xC41B,	0xC5E4,	0xC7B0,	0xC980,	0xCB53,	0xCD2A,	0xCF04,
		0xD0E1,	0xD2C1,	0xD4A4,	0xD68A,	0xD872,	0xDA5D,	0xDC4A,	0xDE39,
		0xE02B,	0xE21E,	0xE414,	0xE60B,	0xE804,	0xE9FE,	0xEBFA,	0xEDF7,
		0xEFF5,	0xF1F4,	0xF3F4,	0xF5F5,	0xF7F6,	0xF9F8,	0xFBFB,	0xFDFD,
};

uint16_t Sine_480Hz[TESTSIGNAL_LENGTH] = {
		0x0000, 0x0809, 0x100A, 0x17FB, 0x1FD4, 0x278D, 0x2F1E, 0x367F,
		0x3DA9,	0x4495,	0x4B3B,	0x5196,	0x579E,	0x5D4E,	0x629F,	0x678D,
		0x6C12,	0x7029,	0x73D0,	0x7701,	0x79BB,	0x7BF9,	0x7DBA,	0x7EFC,
		0x7FBE,	0x7FFF,	0x7FBE,	0x7EFC,	0x7DBA,	0x7BF9,	0x79BB,	0x7701,
		0x73D0,	0x7029,	0x6C12,	0x678D,	0x629F,	0x5D4E,	0x579E,	0x5196,
		0x4B3B,	0x4495,	0x3DA9,	0x367F,	0x2F1E,	0x278D,	0x1FD4,	0x17FB,
		0x100A,	0x0809,	0x0000,	0xF7F7,	0xEFF6,	0xE805,	0xE02C,	0xD873,
		0xD0E2,	0xC981,	0xC257,	0xBB6B,	0xB4C5,	0xAE6A,	0xA862,	0xA2B2,
		0x9D61,	0x9873,	0x93EE,	0x8FD7,	0x8C30,	0x88FF,	0x8645,	0x8407,
		0x8246,	0x8104,	0x8042,	0x8001,	0x8042,	0x8104,	0x8246,	0x8407,
		0x8645,	0x88FF,	0x8C30,	0x8FD7,	0x93EE,	0x9873,	0x9D61,	0xA2B2,
		0xA862,	0xAE6A,	0xB4C5,	0xBB6B,	0xC257,	0xC981,	0xD0E2,	0xD873,
		0xE02C,	0xE805,	0xEFF6,	0x0000,
		0x0000, 0x0809, 0x100A, 0x17FB, 0x1FD4, 0x278D, 0x2F1E, 0x367F,
		0x3DA9,	0x4495,	0x4B3B,	0x5196,	0x579E,	0x5D4E,	0x629F,	0x678D,
		0x6C12,	0x7029,	0x73D0,	0x7701,	0x79BB,	0x7BF9,	0x7DBA,	0x7EFC,
		0x7FBE,	0x7FFF,	0x7FBE,	0x7EFC,	0x7DBA,	0x7BF9,	0x79BB,	0x7701,
		0x73D0,	0x7029,	0x6C12,	0x678D,	0x629F,	0x5D4E,	0x579E,	0x5196,
		0x4B3B,	0x4495,	0x3DA9,	0x367F,	0x2F1E,	0x278D,	0x1FD4,	0x17FB,
		0x100A,	0x0809,	0x0000,	0xF7F7,	0xEFF6,	0xE805,	0xE02C,	0xD873,
		0xD0E2,	0xC981,	0xC257,	0xBB6B,	0xB4C5,	0xAE6A,	0xA862,	0xA2B2,
		0x9D61,	0x9873,	0x93EE,	0x8FD7,	0x8C30,	0x88FF,	0x8645,	0x8407,
		0x8246,	0x8104,	0x8042,	0x8001,	0x8042,	0x8104,	0x8246,	0x8407,
		0x8645,	0x88FF,	0x8C30,	0x8FD7,	0x93EE,	0x9873,	0x9D61,	0xA2B2,
		0xA862,	0xAE6A,	0xB4C5,	0xBB6B,	0xC257,	0xC981,	0xD0E2,	0xD873,
		0xE02C,	0xE805,	0xEFF6,	0x0000,
		0x0000, 0x0809, 0x100A, 0x17FB, 0x1FD4, 0x278D, 0x2F1E, 0x367F,
		0x3DA9,	0x4495,	0x4B3B,	0x5196,	0x579E,	0x5D4E,	0x629F,	0x678D,
		0x6C12,	0x7029,	0x73D0,	0x7701,	0x79BB,	0x7BF9,	0x7DBA,	0x7EFC,
		0x7FBE,	0x7FFF,	0x7FBE,	0x7EFC,	0x7DBA,	0x7BF9,	0x79BB,	0x7701,
		0x73D0,	0x7029,	0x6C12,	0x678D,	0x629F,	0x5D4E,	0x579E,	0x5196,
		0x4B3B,	0x4495,	0x3DA9,	0x367F,	0x2F1E,	0x278D,	0x1FD4,	0x17FB,
		0x100A,	0x0809,	0x0000,	0xF7F7,	0xEFF6,	0xE805,	0xE02C,	0xD873,
		0xD0E2,	0xC981,	0xC257,	0xBB6B,	0xB4C5,	0xAE6A,	0xA862,	0xA2B2,
		0x9D61,	0x9873,	0x93EE,	0x8FD7,	0x8C30,	0x88FF,	0x8645,	0x8407,
		0x8246,	0x8104,	0x8042,	0x8001,	0x8042,	0x8104,	0x8246,	0x8407,
		0x8645,	0x88FF,	0x8C30,	0x8FD7,	0x93EE,	0x9873,	0x9D61,	0xA2B2,
		0xA862,	0xAE6A,	0xB4C5,	0xBB6B,	0xC257,	0xC981,	0xD0E2,	0xD873,
		0xE02C,	0xE805,	0xEFF6,	0x0000,
		0x0000, 0x0809, 0x100A, 0x17FB, 0x1FD4, 0x278D, 0x2F1E, 0x367F,
		0x3DA9,	0x4495,	0x4B3B,	0x5196,	0x579E,	0x5D4E,	0x629F,	0x678D,
		0x6C12,	0x7029,	0x73D0,	0x7701,	0x79BB,	0x7BF9,	0x7DBA,	0x7EFC,
		0x7FBE,	0x7FFF,	0x7FBE,	0x7EFC,	0x7DBA,	0x7BF9,	0x79BB,	0x7701,
		0x73D0,	0x7029,	0x6C12,	0x678D,	0x629F,	0x5D4E,	0x579E,	0x5196,
		0x4B3B,	0x4495,	0x3DA9,	0x367F,	0x2F1E,	0x278D,	0x1FD4,	0x17FB,
		0x100A,	0x0809,	0x0000,	0xF7F7,	0xEFF6,	0xE805,	0xE02C,	0xD873,
		0xD0E2,	0xC981,	0xC257,	0xBB6B,	0xB4C5,	0xAE6A,	0xA862,	0xA2B2,
		0x9D61,	0x9873,	0x93EE,	0x8FD7,	0x8C30,	0x88FF,	0x8645,	0x8407,
		0x8246,	0x8104,	0x8042,	0x8001,	0x8042,	0x8104,	0x8246,	0x8407,
		0x8645,	0x88FF,	0x8C30,	0x8FD7,	0x93EE,	0x9873,	0x9D61,	0xA2B2,
		0xA862,	0xAE6A,	0xB4C5,	0xBB6B,	0xC257,	0xC981,	0xD0E2,	0xD873,
		0xE02C,	0xE805,	0xEFF6,	0x0000,
};
