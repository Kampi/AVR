/*
 * HID_Common.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Common definitions for USB HID class.

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

  Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de.
 */

/** @file USB/Class/HID/HID_Common.h
 *  @brief Common definitions for USB HID class.
 *		   Please read https://www.usb.org/sites/default/files/documents/hid1_11.pdf when you need more information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef HID_COMMON_H_
#define HID_COMMON_H_

 #include "Common.h"

 /** @ingroup 	USB-HID
  *  @brief		USB HID class descriptor types.
  */
 typedef enum 
 {
	 HID_DESCRIPTOR_TYPE_HID = 0x21,									/**< HID class descriptor */
	 HID_DESCRIPTOR_TYPE_REPORT = 0x22,									/**< HID report descriptor */
	 HID_DESCRIPTOR_TYPE_PHYSICAL = 0x22,								/**< HID physical descriptor */
 } USB_HID_DescriptorTypes_t;

 /** @ingroup 	USB-HID
  *  @brief		USB HID class specific requests.
  */
 typedef enum 
 {
	HID_REQUEST_GET_REPORT = 0x01,										/**< Get the current HID report from the device */
	HID_REQUEST_GET_IDLE = 0x02,										/**< Get the current device idle count */
	HID_REQUEST_GET_PROTOCOL = 0x03,									/**< Get the current HID report protocol mode */
	HID_REQUEST_SET_REPORT = 0x09,										/**< Set the current HID report to the device */
	HID_REQUEST_SET_IDLE = 0x0A,										/**< Set the device's idle count */
	HID_REQUEST_SET_PROTOCOL = 0x0B,									/**< Set the current HID report protocol mode */
 } USB_HID_ClassRequests_t;

 /** @ingroup 	USB-HID
  *  @brief 	USB HID country codes.
  */
 typedef enum
 {
	 HID_COUNTRYCODE_NOT_SUPPORTED = 0x00,								/**< Country code: Not supported country */
	 HID_COUNTRYCODE_ARABIC = 0x01,										/**< Country code: Arabic */
	 HID_COUNTRYCODE_BELGIAN = 0x02,									/**< Country code: Belgian */
	 HID_COUNTRYCODE_CANADIAN_BI = 0x03,								/**< Country code: Canadian-Bilingual */
	 HID_COUNTRYCODE_CANADIAN_FR = 0x04,								/**< Country code: Canadian-French */
	 HID_COUNTRYCODE_CZECH = 0x05,										/**< Country code: Czech Republic */
	 HID_COUNTRYCODE_DANISH = 0x06,										/**< Country code: Danish */
	 HID_COUNTRYCODE_FINNISH = 0x07,									/**< Country code: Finnish */
	 HID_COUNTRYCODE_FRENCH = 0x08,										/**< Country code: French */
	 HID_COUNTRYCODE_GERMAN = 0x09,										/**< Country code: German */
	 HID_COUNTRYCODE_GREEK = 0x0A,										/**< Country code: Greek */
	 HID_COUNTRYCODE_HEBREW = 0x0B,										/**< Country code: Hebrew */
	 HID_COUNTRYCODE_HUNGARY = 0x0C,									/**< Country code: Hungary */
	 HID_COUNTRYCODE_INTERNATIONAL = 0x0D,								/**< Country code: International */
	 HID_COUNTRYCODE_ITALIAN = 0x0E,									/**< Country code: Italian */
	 HID_COUNTRYCODE_JAPAN = 0x0F,										/**< Country code: Japan */
	 HID_COUNTRYCODE_KOREAN = 0x10,										/**< Country code: Korean */
	 HID_COUNTRYCODE_LATIN_AMERICAN = 0x11,								/**< Country code: Latin American */
	 HID_COUNTRYCODE_NETHERLANDS = 0x12,								/**< Country code: Netherlands/Dutch */
	 HID_COUNTRYCODE_NORWEGIAN = 0x13,									/**< Country code: Norwegian */
	 HID_COUNTRYCODE_PERSIAN = 0x14,									/**< Country code: Persian */
	 HID_COUNTRYCODE_POLAND = 0x15,										/**< Country code: Poland */
	 HID_COUNTRYCODE_PORTUGUESE = 0x16,									/**< Country code: Portuguese */
	 HID_COUNTRYCODE_RUSSIA = 0x17,										/**< Country code: Russia */
	 HID_COUNTRYCODE_SLOVAKIA = 0x18,									/**< Country code: Slovakia */
	 HID_COUNTRYCODE_SPANISH = 0x19,									/**< Country code: Spanish */
	 HID_COUNTRYCODE_SWEDISH = 0x1A,									/**< Country code: Swedish */
	 HID_COUNTRYCODE_SWISS_FR = 0x1B,									/**< Country code: Swiss/French */
	 HID_COUNTRYCODE_SWISS_GER = 0x1C,									/**< Country code: Swiss/German */
	 HID_COUNTRYCODE_SWITZERLAND = 0x1D,								/**< Country code: Switzerland */
	 HID_COUNTRYCODE_TAIWAN = 0x1E,										/**< Country code: Taiwan */
	 HID_COUNTRYCODE_TURKISH_Q = 0x1F,									/**< Country code: Turkish-Q */
	 HID_COUNTRYCODE_UK = 0x20,											/**< Country code: UK */
	 HID_COUNTRYCODE_US = 0x21,											/**< Country code: US */
	 HID_COUNTRYCODE_YUGOSLAVIA = 0x22,									/**< Country code: Yugoslavia */
	 HID_COUNTRYCODE_TURKISH_F = 0x23,									/**< Country code: Turkish-F */
 } USB_HID_CountryCode_t;

 /** @ingroup	USB-HID
  *  @brief		USB HID subclasses codes used by the \ref USB_InterfaceDescriptor_t.bInterfaceSubClass field.
  */
 typedef enum
 {
	 HID_SUBCLASS_NONE = 0x00,											/**< No subclass */
	 HID_SUBCLASS_BOOT_INTERFACE = 0x01,								/**< Boot interface subclass */
 } USB_HID_SubClass_t;

 /** @ingroup	USB-HID
  *  @brief		USB HID protocol codes used by the \ref USB_InterfaceDescriptor_t.bInterfaceProtocol field.
  *  			NOTE: Only needed if you choose #USB_SUBCLASS_BOOT_INTERFACE in the Interface Descriptor. \n
  *					  Otherwise set it to #USB_PROTOCOL_NONE.
  */
 typedef enum
 {
	 HID_PROTOCOL_NONE = 0x00,											/**< No protocol */
	 HID_PROTOCOL_KEYBOARD = 0x01,										/**< Keyboard protocol */
	 HID_PROTOCOL_MOUSE = 0x02,											/**< Mouse protocol */
 } USB_HID_Protocol_t;

 /** @ingroup	USB-HID
  *  @brief		USB HID report types.
  */
 typedef enum
 {
	 HID_REPORT_TYPE_INPUT = 0x01,										/**< Input report */
	 HID_REPORT_TYPE_OUTPUT = 0x02,										/**< Output report */
	 HID_REPORT_TYPE_FEATURE = 0x03,									/**< Feature report */
 } USB_HID_ReportType_t;

 /** @ingroup	USB-HID-Keyboard
  *  @brief		USB HID keyboard scan codes.
  */
 typedef enum
 {
	 HID_KEYBOARD_SC_NONE = 0x00,										/**< No key pressed */
	 HID_KEYBOARD_SC_ERROR_ROLLOVER = 0x01,								/**< Keyboard Error Roll Over - used for all slots if too many keys are pressed. */
	 HID_KEYBOARD_SC_POST_FAIL = 0x02,									/**< */
	 HID_KEYBOARD_SC_ERROR_UNDEFINED = 0x03,							/**< Undefined error */
	 HID_KEYBOARD_SC_A = 0x04,											/**< A and a */
	 HID_KEYBOARD_SC_B = 0x05,											/**< B and b */
	 HID_KEYBOARD_SC_C = 0x06,											/**< C and c */
	 HID_KEYBOARD_SC_D = 0x07,											/**< D and d */
	 HID_KEYBOARD_SC_E = 0x08,											/**< E and e */
	 HID_KEYBOARD_SC_F = 0x09,											/**< F and f */
	 HID_KEYBOARD_SC_G = 0x0A,											/**< G and g */
	 HID_KEYBOARD_SC_H = 0x0B,											/**< H and h */
	 HID_KEYBOARD_SC_I = 0x0C,											/**< I and i */
	 HID_KEYBOARD_SC_J = 0x0D,											/**< J and j */
 	 HID_KEYBOARD_SC_K = 0x0E,											/**< K and k */
	 HID_KEYBOARD_SC_L = 0x0F,											/**< L and l */
	 HID_KEYBOARD_SC_M = 0x10,											/**< M and m */
	 HID_KEYBOARD_SC_N = 0x11,											/**< N and n */
 	 HID_KEYBOARD_SC_O = 0x12,											/**< O and o */
	 HID_KEYBOARD_SC_P = 0x13,											/**< P and p */
	 HID_KEYBOARD_SC_Q = 0x14,											/**< Q and q */
	 HID_KEYBOARD_SC_R = 0x15,											/**< R and r */
	 HID_KEYBOARD_SC_S = 0x16,											/**< S and s */
 	 HID_KEYBOARD_SC_T = 0x17,											/**< T and t */
	 HID_KEYBOARD_SC_U = 0x18,											/**< U and u */
	 HID_KEYBOARD_SC_V = 0x19,											/**< V and v */
	 HID_KEYBOARD_SC_W = 0x1A,											/**< W and w */
	 HID_KEYBOARD_SC_X = 0x1B,											/**< X and x */
	 HID_KEYBOARD_SC_Y = 0x1C,											/**< Y and y */
	 HID_KEYBOARD_SC_Z = 0x1D,											/**< Z and z */
	 HID_KEYBOARD_SC_1_AND_EXCLAMATION = 0x1E,
	 HID_KEYBOARD_SC_2_AND_AT = 0x1F,
	 HID_KEYBOARD_SC_3_AND_HASHMARK = 0x20,
	 HID_KEYBOARD_SC_4_AND_DOLLAR = 0x21,
	 HID_KEYBOARD_SC_5_AND_PERCENTAGE = 0x22,
	 HID_KEYBOARD_SC_6_AND_CARET = 0x23,
	 HID_KEYBOARD_SC_7_AND_AMPERSAND = 0x24,
	 HID_KEYBOARD_SC_8_AND_ASTERISK = 0x25,
	 HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS = 0x26,
	 HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS = 0x27,
	 HID_KEYBOARD_SC_ENTER = 0x28,
	 HID_KEYBOARD_SC_ESCAPE = 0x29,
	 HID_KEYBOARD_SC_BACKSPACE = 0x2A,
	 HID_KEYBOARD_SC_TAB = 0x2B,
	 HID_KEYBOARD_SC_SPACE = 0x2C,
	 HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE = 0x2D,
	 HID_KEYBOARD_SC_EQUAL_AND_PLUS = 0x2E,
	 HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE = 0x2F,
	 HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE = 0x30,
	 HID_KEYBOARD_SC_BACKSLASH_AND_PIPE = 0x31,
	 HID_KEYBOARD_SC_NON_US_HASHMARK_AND_TILDE = 0x32,
	 HID_KEYBOARD_SC_SEMICOLON_AND_COLON = 0x33,
	 HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE = 0x34,
	 HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE = 0x35,
	 HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN = 0x36,
	 HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN = 0x37,
	 HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK = 0x38,
	 HID_KEYBOARD_SC_CAPS_LOCK = 0x39,
	 HID_KEYBOARD_SC_F1 = 0x3A,
	 HID_KEYBOARD_SC_F2 = 0x3B,
	 HID_KEYBOARD_SC_F3 = 0x3C,
	 HID_KEYBOARD_SC_F4 = 0x3D,
	 HID_KEYBOARD_SC_F5 = 0x3E,
	 HID_KEYBOARD_SC_F6 = 0x3F,
	 HID_KEYBOARD_SC_F7 = 0x40,
	 HID_KEYBOARD_SC_F8 = 0x41,
	 HID_KEYBOARD_SC_F9 = 0x42,
	 HID_KEYBOARD_SC_F10 = 0x43,
	 HID_KEYBOARD_SC_F11 = 0x44,
	 HID_KEYBOARD_SC_F12 = 0x45,
	 HID_KEYBOARD_SC_PRINT_SCREEN = 0x46,
	 HID_KEYBOARD_SC_SCROLL_LOCK = 0x47,
	 HID_KEYBOARD_SC_PAUSE = 0x48,
	 HID_KEYBOARD_SC_INSERT = 0x49,
	 HID_KEYBOARD_SC_HOME = 0x4A,
	 HID_KEYBOARD_SC_PAGE_UP = 0x4B,
	 HID_KEYBOARD_SC_DELETE = 0x4C,
	 HID_KEYBOARD_SC_END = 0x4D,
	 HID_KEYBOARD_SC_PAGE_DOWN = 0x4E,
	 HID_KEYBOARD_SC_RIGHT_ARROW = 0x4F,
	 HID_KEYBOARD_SC_LEFT_ARROW = 0x50,
	 HID_KEYBOARD_SC_DOWN_ARROW = 0x51,
	 HID_KEYBOARD_SC_UP_ARROW = 0x52,
	 HID_KEYBOARD_SC_NUM_LOCK = 0x53,
	 HID_KEYBOARD_SC_KEYPAD_SLASH = 0x54,
	 HID_KEYBOARD_SC_KEYPAD_ASTERISK = 0x55,
	 HID_KEYBOARD_SC_KEYPAD_MINUS = 0x56,
	 HID_KEYBOARD_SC_KEYPAD_PLUS = 0x57,
	 HID_KEYBOARD_SC_KEYPAD_ENTER = 0x58,
	 HID_KEYBOARD_SC_KEYPAD_1 = 0x59,
	 HID_KEYBOARD_SC_KEYPAD_2 = 0x5A,
	 HID_KEYBOARD_SC_KEYPAD_3 = 0x5B,
	 HID_KEYBOARD_SC_KEYPAD_4 = 0x5C,
	 HID_KEYBOARD_SC_KEYPAD_5 = 0x5D,
	 HID_KEYBOARD_SC_KEYPAD_6 = 0x5E,
	 HID_KEYBOARD_SC_KEYPAD_7 = 0x5F,
	 HID_KEYBOARD_SC_KEYPAD_8 = 0x60,
	 HID_KEYBOARD_SC_KEYPAD_9 = 0x61,
	 HID_KEYBOARD_SC_KEYPAD_0 = 0x62,
	 HID_KEYBOARD_SC_KEYPAD_DOT = 0x63,
	 HID_KEYBOARD_SC_NON_US_BACKSLASH_AND_PIPE = 0x64,
	 HID_KEYBOARD_SC_APPLICATION = 0x65,
	 HID_KEYBOARD_SC_POWER = 0x66,
	 HID_KEYBOARD_SC_KEYPAD_EQUAL_SIGN = 0x67,
	 HID_KEYBOARD_SC_F13 = 0x68,
	 HID_KEYBOARD_SC_F14 = 0x69,
	 HID_KEYBOARD_SC_F15 = 0x6A,
	 HID_KEYBOARD_SC_F16 = 0x6B,
	 HID_KEYBOARD_SC_F17 = 0x6C,
	 HID_KEYBOARD_SC_F18 = 0x6D,
	 HID_KEYBOARD_SC_F19 = 0x6E,
	 HID_KEYBOARD_SC_F20 = 0x6F,
	 HID_KEYBOARD_SC_F21 = 0x70,
	 HID_KEYBOARD_SC_F22 = 0x71,
	 HID_KEYBOARD_SC_F23 = 0x72,
	 HID_KEYBOARD_SC_F24 = 0x73,
	 HID_KEYBOARD_SC_EXECUTE = 0x74,
	 HID_KEYBOARD_SC_HELP = 0x75,
	 HID_KEYBOARD_SC_MENU = 0x76,
	 HID_KEYBOARD_SC_SELECT = 0x77,
	 HID_KEYBOARD_SC_STOP = 0x78,
	 HID_KEYBOARD_SC_AGAIN = 0x79,
	 HID_KEYBOARD_SC_UNDO = 0x7A,
	 HID_KEYBOARD_SC_CUT = 0x7B,
	 HID_KEYBOARD_SC_COPY = 0x7C,
	 HID_KEYBOARD_SC_PASTE = 0x7D,
	 HID_KEYBOARD_SC_FIND = 0x7E,
	 HID_KEYBOARD_SC_MUTE = 0x7F,
	 HID_KEYBOARD_SC_VOLUME_UP = 0x80,
	 HID_KEYBOARD_SC_VOLUME_DOWN = 0x81,
	 HID_KEYBOARD_SC_LOCKING_CAPS_LOCK = 0x82,
	 HID_KEYBOARD_SC_LOCKING_NUM_LOCK = 0x83,
	 HID_KEYBOARD_SC_LOCKING_SCROLL_LOCK = 0x84,
	 HID_KEYBOARD_SC_KEYPAD_COMMA = 0x85,
	 HID_KEYBOARD_SC_KEYPAD_EQUAL_SIGN_AS400 = 0x86,
	 HID_KEYBOARD_SC_INTERNATIONAL1 = 0x87,
	 HID_KEYBOARD_SC_INTERNATIONAL2 = 0x88,
	 HID_KEYBOARD_SC_INTERNATIONAL3 = 0x89,
	 HID_KEYBOARD_SC_INTERNATIONAL4 = 0x8A,
	 HID_KEYBOARD_SC_INTERNATIONAL5 = 0x8B,
	 HID_KEYBOARD_SC_INTERNATIONAL6 = 0x8C,
	 HID_KEYBOARD_SC_INTERNATIONAL7 = 0x8D,
	 HID_KEYBOARD_SC_INTERNATIONAL8 = 0x8E,
	 HID_KEYBOARD_SC_INTERNATIONAL9 = 0x8F,
	 HID_KEYBOARD_SC_LANG1 = 0x90,
	 HID_KEYBOARD_SC_LANG2 = 0x91,
	 HID_KEYBOARD_SC_LANG3 = 0x92,
	 HID_KEYBOARD_SC_LANG4 = 0x93,
	 HID_KEYBOARD_SC_LANG5 = 0x94,
	 HID_KEYBOARD_SC_LANG6 = 0x95,
	 HID_KEYBOARD_SC_LANG7 = 0x96,
	 HID_KEYBOARD_SC_LANG8 = 0x97,
	 HID_KEYBOARD_SC_LANG9 = 0x98,
	 HID_KEYBOARD_SC_ALTERNATE_ERASE = 0x99,
	 HID_KEYBOARD_SC_SYSREQ = 0x9A,
	 HID_KEYBOARD_SC_CANCEL = 0x9B,
	 HID_KEYBOARD_SC_CLEAR = 0x9C,
	 HID_KEYBOARD_SC_PRIOR = 0x9D,
	 HID_KEYBOARD_SC_RETURN = 0x9E,
	 HID_KEYBOARD_SC_SEPARATOR = 0x9F,
	 HID_KEYBOARD_SC_OUT = 0xA0,
	 HID_KEYBOARD_SC_OPER = 0xA1,
	 HID_KEYBOARD_SC_CLEAR_AND_AGAIN = 0xA2,
	 HID_KEYBOARD_SC_CRSEL_AND_PROPS = 0xA3,
	 HID_KEYBOARD_SC_EXSEL = 0xA4,
	 HID_KEYBOARD_SC_KEYPAD_00 = 0xB0,
	 HID_KEYBOARD_SC_KEYPAD_000 = 0xB1,
	 HID_KEYBOARD_SC_THOUSANDS_SEPARATOR = 0xB2,
	 HID_KEYBOARD_SC_DECIMAL_SEPARATOR = 0xB3,
	 HID_KEYBOARD_SC_CURRENCY_UNIT = 0xB4,
	 HID_KEYBOARD_SC_CURRENCY_SUB_UNIT = 0xB5,
	 HID_KEYBOARD_SC_KEYPAD_OPENING_PARENTHESIS = 0xB6,
	 HID_KEYBOARD_SC_KEYPAD_CLOSING_PARENTHESIS = 0xB7,
	 HID_KEYBOARD_SC_KEYPAD_OPENING_BRACE = 0xB8,
	 HID_KEYBOARD_SC_KEYPAD_CLOSING_BRACE = 0xB9,
	 HID_KEYBOARD_SC_KEYPAD_TAB = 0xBA,
	 HID_KEYBOARD_SC_KEYPAD_BACKSPACE = 0xBB,
	 HID_KEYBOARD_SC_KEYPAD_A = 0xBC,
	 HID_KEYBOARD_SC_KEYPAD_B = 0xBD,
	 HID_KEYBOARD_SC_KEYPAD_C = 0xBE,
	 HID_KEYBOARD_SC_KEYPAD_D = 0xBF,
	 HID_KEYBOARD_SC_KEYPAD_E = 0xC0,
	 HID_KEYBOARD_SC_KEYPAD_F = 0xC1,
	 HID_KEYBOARD_SC_KEYPAD_XOR = 0xC2,
	 HID_KEYBOARD_SC_KEYPAD_CARET = 0xC3,
	 HID_KEYBOARD_SC_KEYPAD_PERCENTAGE = 0xC4,
	 HID_KEYBOARD_SC_KEYPAD_LESS_THAN_SIGN = 0xC5,
	 HID_KEYBOARD_SC_KEYPAD_GREATER_THAN_SIGN = 0xC6,
	 HID_KEYBOARD_SC_KEYPAD_AMP = 0xC7,
	 HID_KEYBOARD_SC_KEYPAD_AMP_AMP = 0xC8,
	 HID_KEYBOARD_SC_KEYPAD_PIPE = 0xC9,
	 HID_KEYBOARD_SC_KEYPAD_PIPE_PIPE = 0xCA,
	 HID_KEYBOARD_SC_KEYPAD_COLON = 0xCB,
	 HID_KEYBOARD_SC_KEYPAD_HASHMARK = 0xCC,
	 HID_KEYBOARD_SC_KEYPAD_SPACE = 0xCD,
	 HID_KEYBOARD_SC_KEYPAD_AT = 0xCE,
	 HID_KEYBOARD_SC_KEYPAD_EXCLAMATION_SIGN = 0xCF,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_STORE = 0xD0,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_RECALL = 0xD1,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_CLEAR = 0xD2,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_ADD = 0xD3,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_SUBTRACT = 0xD4,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_MULTIPLY = 0xD5,
	 HID_KEYBOARD_SC_KEYPAD_MEMORY_DIVIDE = 0xD6,
	 HID_KEYBOARD_SC_KEYPAD_PLUS_AND_MINUS = 0xD7,
	 HID_KEYBOARD_SC_KEYPAD_CLEAR = 0xD8,
	 HID_KEYBOARD_SC_KEYPAD_CLEAR_ENTRY = 0xD9,
	 HID_KEYBOARD_SC_KEYPAD_BINARY = 0xDA,
	 HID_KEYBOARD_SC_KEYPAD_OCTAL = 0xDB,
	 HID_KEYBOARD_SC_KEYPAD_DECIMAL = 0xDC,
	 HID_KEYBOARD_SC_KEYPAD_HEXADECIMAL = 0xDD,
	 HID_KEYBOARD_SC_LEFT_CONTROL = 0xE0,
	 HID_KEYBOARD_SC_LEFT_SHIFT = 0xE1,
	 HID_KEYBOARD_SC_LEFT_ALT = 0xE2,
	 HID_KEYBOARD_SC_LEFT_GUI = 0xE3,
	 HID_KEYBOARD_SC_RIGHT_CONTROL = 0xE4,
	 HID_KEYBOARD_SC_RIGHT_SHIFT = 0xE5,
	 HID_KEYBOARD_SC_RIGHT_ALT = 0xE6,
	 HID_KEYBOARD_SC_RIGHT_GUI = 0xE7,
	 HID_KEYBOARD_SC_MEDIA_PLAY = 0xE8,
	 HID_KEYBOARD_SC_MEDIA_STOP = 0xE9,
	 HID_KEYBOARD_SC_MEDIA_PREVIOUS_TRACK = 0xEA,
	 HID_KEYBOARD_SC_MEDIA_NEXT_TRACK = 0xEB,
	 HID_KEYBOARD_SC_MEDIA_EJECT = 0xEC,
	 HID_KEYBOARD_SC_MEDIA_VOLUME_UP = 0xED,
	 HID_KEYBOARD_SC_MEDIA_VOLUME_DOWN = 0xEE,
	 HID_KEYBOARD_SC_MEDIA_MUTE = 0xEF,
	 HID_KEYBOARD_SC_MEDIA_WWW = 0xF0,
	 HID_KEYBOARD_SC_MEDIA_BACKWARD = 0xF1,
	 HID_KEYBOARD_SC_MEDIA_FORWARD = 0xF2,
	 HID_KEYBOARD_SC_MEDIA_CANCEL = 0xF3,
	 HID_KEYBOARD_SC_MEDIA_SEARCH = 0xF4,
	 HID_KEYBOARD_SC_MEDIA_SLEEP = 0xF8,
	 HID_KEYBOARD_SC_MEDIA_LOCK = 0xF9,
	 HID_KEYBOARD_SC_MEDIA_RELOAD = 0xFA,
	 HID_KEYBOARD_SC_MEDIA_CALCULATOR = 0xFB,
 } USB_HID_Keyboard_ScanCode_t;

 /** @ingroup 	USB-HID-Keyboard
  *  @brief		USB HID keyboard LEDs.
  */
 typedef enum
 {
	 HID_KEYBOARD_LED_NUM_LOCK = (0x01 << 0x00),						/**< NUM lock LED */
	 HID_KEYBOARD_LED_CAPS_LOCK = (0x01 << 0x01),						/**< CAPS lock LED */
	 HID_KEYBOARD_LED_SCROLL_LOCK = (0x01 << 0x02),						/**< SCROLL lock LED */
	 HID_KEYBOARD_LED_COMPOSE = (0x01 << 0x03),							/**< COMPOSE LED */
	 HID_KEYBOARD_LED_KANA = (0x01 << 0x04),							/**< KANA LED */
 } USB_HID_Keyboard_LED_t;

 /** @ingroup 	USB-HID-Keyboard
  *  @brief		USB HID keyboard modifier used by the \ref USB_KeyboardReport_t.Modifier field.
  */
 typedef enum
 {
	 HID_KEYBOARD_MODIFIER_CTRL_LEFT = (0x01 << 0x00),					/**< Left CTRL button */
	 HID_KEYBOARD_MODIFIER_SHIFT_LEFT = (0x01 << 0x01),					/**< Left SHIFT button */
	 HID_KEYBOARD_MODIFIER_ALT_LEFT = (0x01 << 0x02),					/**< Left ALT button */
	 HID_KEYBOARD_MODIFIER_GUI_LEFT = (0x01 << 0x03),					/**< Left Windows or CMD button */
	 HID_KEYBOARD_MODIFIER_CTRL_RIGHT = (0x01 << 0x04),					/**< Left CTRL button */
	 HID_KEYBOARD_MODIFIER_SHIFT_RIGHT = (0x01 << 0x05),				/**< Left SHIFT button */
	 HID_KEYBOARD_MODIFIER_ALT_RIGHT = (0x01 << 0x06),					/**< Left ALT button */
	 HID_KEYBOARD_MODIFIER_GUI_RIGHT = (0x01 << 0x07),					/**< Left Windows or CMD button */
 } USB_HID_Keyboard_Modifier_t;

 /** @ingroup 	USB-HID-Mouse
  *  @brief		USB HID mouse buttons used by the \ref USB_MouseReport_t.Button field.
  */
 typedef enum 
 {
	 HID_MOUSE_BUTTON_LEFT = 0x01,										/**< Left mouse button */
	 HID_MOUSE_BUTTON_RIGHT = 0x02,										/**< Right mouse button */
	 HID_MOUSE_BUTTON_MIDDLE = 0x04,									/**< Middle mouse button */
 } USB_HID_Mouse_Button_t;

 /** @ingroup 	USB-HID
  *  @brief 	USB HID descriptor definition.
  */
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< HID descriptor type (=33) */
	 uint16_t bcdHID;													/**< HID Class Spec Version */
	 uint8_t bCountryCode;												/**< Country code. Zero if universal */
	 uint8_t bNumDescriptors;											/**< Number of Descriptors */
	 uint8_t bReportType;												/**< Type of HID report (=34) */
	 uint16_t wDescriptorLength;										/**< Descriptor length */
 } __attribute__((packed)) USB_HID_Descriptor_t;

 /** @ingroup 	USB-HID-Mouse
  *  @brief		Standard HID mouse report definition.
  */
 typedef struct
 {
	 uint8_t Button;													/**< Button mask for currently pressed buttons */
	 int8_t X;															/**< Delta X movement */
	 int8_t Y;															/**< Delta Y movement */
 } __attribute__((packed)) USB_MouseReport_t;

 /** @ingroup 	USB-HID-Keyboard
  *  @brief		Standard HID keyboard report definition.
  */
 typedef struct
 {
	 uint8_t Modifier;													/**< Keyboard modifier byte */
	 uint8_t Reserved;													/**< Reserved. Always set to 0 */
	 uint8_t KeyCode[6];												/**< Key codes of the pressed keys */
 } __attribute__((packed)) USB_KeyboardReport_t;

#endif /* HID_COMMON_H_ */