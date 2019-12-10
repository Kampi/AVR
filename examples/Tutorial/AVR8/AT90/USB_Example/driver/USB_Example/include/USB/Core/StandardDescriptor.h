/*
 * DescriptorTypes.h
 *
 *  Copyright (C) Daniel Kampert, 2018
 *	Website: www.kampis-elektroecke.de
 *  File info: Standard USB Descriptor definitions.

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

  Errors and omissions should be reported to DanielKampert@kampis-elektroecke.de
 */

/** @file USB/Core/StandardDescriptor.h
 *  @brief Standard USB Descriptor definitions.
 *		   See http://www.usbmadesimple.co.uk/ums_4.htm for additional information.
 *
 *  @author Daniel Kampert
 *  @bug No known bugs
 */

#ifndef DESCRIPTORTYPES_H_
#define DESCRIPTORTYPES_H_

 #include "USB/Core/Common.h"

 /** @brief	Macro to convert multiple bytes to a Unicode string descriptor.
 */
 #define LANG_TO_STRING_DESCRIPTOR(...)					{ .bLength = sizeof(USB_StringDescriptor_t) + (uint16_t)VA_NARGS(__VA_ARGS__), .bDescriptorType = DESCRIPTOR_TYPE_STRING, .bString = {__VA_ARGS__} }

 /** @brief	Macro to convert a char array to a Unicode string descriptor.
 */
 #define WCHAR_TO_STRING_DESCRIPTOR(Array)				{ .bLength = sizeof(USB_StringDescriptor_t) + (sizeof(Array) - 2), .bDescriptorType = DESCRIPTOR_TYPE_STRING, .bString = Array }

 /** @brief	Macro to convert a primary language and a sub language into a 16-bit language id.
 */
 #define CONV_LANG(Primary, Sub)						(((Sub & 0x3F) << 10) | (Primary & 0x3FF))

 /** @brief	Macro to convert a USB BCD version number.
 */
 #define USB_VERSION(Revision, Minor, Major)			((Major & 0xFF) << 0x08) | ((Minor & 0x0F) << 0x04) | (Revision & 0x0F)

 /** @brief	Macro to create the value for the maximum power consumption of a device in mA.
 */
 #define USB_CURRENT_CONSUMPTION(Current)				((Current) >> 0x01)

 /** @brief	Macro to create the USB configuration descriptor bmAttributes bitmap from a given #USB_ConfigAttributes_t mask.
 */
 #define USB_MASK2CONFIG(Mask)							(0x80 | Mask)

 /** @brief Macro to create the USB endpoint descriptor bmAttributes bitmap from a given #USB_EndpointAttributes_t mask.
  */
 #define USB_MASK2ENDPOINT(Usage, Synch, Transfer)		(Usage << 0x04 | Synch << 0x02 | Transfer)

 /** @ingroup USB
  *  USB descriptor types */
 /*\@{*/
 typedef enum
 {
	 DESCRIPTOR_TYPE_DEVICE = 0x01,										/**< Device descriptor ID */ 
	 DESCRIPTOR_TYPE_CONFIGURATION = 0x02,								/**< Configuration descriptor ID */ 
	 DESCRIPTOR_TYPE_STRING = 0x03,										/**< String descriptor ID */ 
	 DESCRIPTOR_TYPE_INTERFACE = 0x04,									/**< Interface descriptor ID */ 
	 DESCRIPTOR_TYPE_ENDPOINT = 0x05,									/**< Endpoint descriptor ID */ 
 } DescriptorTypes_t;
 /*\@}*/

 /** @ingroup USB
  *  USB base classes
  *  NOTE: Visit https://www.usb.org/defined-class-codes for more information. */
 /*\@{*/
 typedef enum
 {
	 USB_CLASS_USE_INTERFACE = 0x00,									/**< Use the USB Interface Descriptor for the class information 
																			 NOTE: Use this in the Device Descriptor */
	 USB_CLASS_AUDIO = 0x01,											/**< Audio device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_CDC = 0x02,												/**< Communications and CDC Control class
																			 NOTE: Use this in the Device and Interface Descriptor */
	 USB_CLASS_HID = 0x03,												/**< HID device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_PHYSICAL = 0x05,											/**< Physical device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_IMAGE = 0x06,											/**< Image device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_PRINTER = 0x07,											/**< Printer device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_MASS_STORAGE = 0x08,										/**< Mass storage device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_HUB = 0x08,												/**< Hub device class
																			 NOTE: Use this in the Device Descriptor */
	 USB_CLASS_CDC_DATA = 0x0A,											/**< CDC data device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_SMARTCARD = 0x0B,										/**< Smart card device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_SECURITY = 0x0D,											/**< Content Security device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_VIDEO = 0x0E,											/**< Video device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_HEALTHCARE = 0x0F,										/**< Healthcare device class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_AUDIO_VIDEO = 0x10,										/**< Audio/Video device class
																			 NOTE: Use this in the Interface Descriptor */
	 USB_CLASS_BILLBOARD = 0x11,										/**< Billboard device class
																			 NOTE: Use this in the Device Descriptor */
 	 USB_CLASS_USB_C_BRIDGE = 0x12,										/**< USB Type-C Bridge class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_DIAGNOSTIC = 0xDC,										/**< Diagnostic class
																			 NOTE: Use this in the Device and Interface Descriptor */
 	 USB_CLASS_WIRELESS = 0xE0,											/**< Wireless controller device class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_MISC = 0xEF,												/**< Miscellaneous device class
																			 NOTE: Use this in the Device and Interface Descriptor */
 	 USB_CLASS_APPLICATION = 0xFE,										/**< Application specific device class
																			 NOTE: Use this in the Interface Descriptor */
 	 USB_CLASS_VENDOR = 0xFF,											/**< Vendor specific device class
																			 NOTE: Use this in the Device and Interface Descriptor */
 } USB_BaseClass_t;
 /*\@}*/

 /** @ingroup USB
  *  USB primary language ID codes */
 /*\@{*/
 typedef enum
 {
	 LANG_ARABIC = 0x01,                 			 					/**< Primary language: Arabic */
	 LANG_BULGARIAN = 0x02,              			 					/**< Primary language: Bulgarian */
	 LANG_CATALAN = 0x03,                			 					/**< Primary language: Catalan */
	 LANG_CHINESE = 0x04,                			 					/**< Primary language: Chinese */
	 LANG_CZECH = 0x05,                  			 					/**< Primary language: Czech */
	 LANG_DANISH = 0x06,                 			 					/**< Primary language: Danish */
	 LANG_GERMAN = 0x07,                 			 					/**< Primary language: German */
	 LANG_GREEK = 0x08,                  			 					/**< Primary language: Greek */
	 LANG_ENGLISH = 0x09,                			 					/**< Primary language: English */
	 LANG_SPANISH = 0x0a,                			 					/**< Primary language: Spanish */
	 LANG_FINNISH = 0x0b,                			 					/**< Primary language: Finnish */
	 LANG_FRENCH = 0x0c,                 			 					/**< Primary language: French */
	 LANG_HEBREW = 0x0d,                 			 					/**< Primary language: Hebrew */
	 LANG_HUNGARIAN = 0x0e,              			 					/**< Primary language: Hungarian */
	 LANG_ICELANDIC = 0x0f,              			 					/**< Primary language: Icelandic */
	 LANG_ITALIAN = 0x10,                			 					/**< Primary language: Italian */
	 LANG_JAPANESE = 0x11,               			 					/**< Primary language: Japanese */
	 LANG_KOREAN = 0x12,                 			 					/**< Primary language: Korean */
	 LANG_DUTCH = 0x13,                  			 					/**< Primary language: Dutch */
	 LANG_NORWEGIAN = 0x14,              			 					/**< Primary language: Norwegian */
	 LANG_POLISH = 0x15,                 			 					/**< Primary language: Polish */
	 LANG_PORTUGUESE = 0x16,             			 					/**< Primary language: Portuguese */
	 LANG_ROMANIAN = 0x18,               			 					/**< Primary language: Romanian */
	 LANG_RUSSIAN = 0x19,                			 					/**< Primary language: Russian */
	 LANG_CROATIAN = 0x1a,               			 					/**< Primary language: Croatian */
	 LANG_SERBIAN = 0x1a,                			 					/**< Primary language: Serbian */
	 LANG_SLOVAK = 0x1b,                 			 					/**< Primary language: Slovak */
	 LANG_ALBANIAN = 0x1c,               			 					/**< Primary language: Albanian */
	 LANG_SWEDISH = 0x1d,                			 					/**< Primary language: Swedish */
	 LANG_THAI = 0x1e,                   			 					/**< Primary language: Thai */
	 LANG_TURKISH = 0x1f,                			 					/**< Primary language: Turkish */
	 LANG_URDU = 0x20,                   			 					/**< Primary language: Urdu */
	 LANG_INDONESIAN = 0x21,             			 					/**< Primary language: Indonesian */
	 LANG_UKRANIAN = 0x22,               			 					/**< Primary language: Ukrainian */
	 LANG_BELARUSIAN = 0x23,             			 					/**< Primary language: Belarusian */
	 LANG_SLOVENIAN = 0x24,              			 					/**< Primary language: Slovenian */
	 LANG_ESTONIAN = 0x25,               			 					/**< Primary language: Estonian */
	 LANG_LATVIAN = 0x26,                			 					/**< Primary language: Latvian */
	 LANG_LITHUANIAN = 0x27,             			 					/**< Primary language: Lithuanian */
	 LANG_FARSI = 0x29,                  			 					/**< Primary language: Farsi */
	 LANG_VIETNAMESE = 0x2a,             			 					/**< Primary language: Vietnamese */
	 LANG_ARMENIAN = 0x2b,               			 					/**< Primary language: Armenian */
	 LANG_AZERI = 0x2c,                  			 					/**< Primary language: Azeri */
	 LANG_BASQUE = 0x2d,                 			 					/**< Primary language: Basque */
	 LANG_MACEDONIAN = 0x2f,             			 					/**< Primary language: Macedonian */
	 LANG_AFRIKAANS = 0x36,              			 					/**< Primary language: Afrikaans */
	 LANG_GEORGIAN = 0x37,               			 					/**< Primary language: Georgian */
	 LANG_FAEROESE = 0x38,               			 					/**< Primary language: Faeroese */
	 LANG_HINDI = 0x39,                  			 					/**< Primary language: Hindi */
	 LANG_MALAY = 0x3e,                  			 					/**< Primary language: Malay */
	 LANG_KAZAK = 0x3f,                  			 					/**< Primary language: Kazak */
	 LANG_SWAHILI = 0x41,                			 					/**< Primary language: Swahili */
	 LANG_UZBEK = 0x43,                  			 					/**< Primary language: Uzbek */
	 LANG_TATAR = 0x44,                  			 					/**< Primary language: Tatar */
	 LANG_BENGALI = 0x45,                			 					/**< Primary language: Bengali */
	 LANG_PUNJABI = 0x46,                			 					/**< Primary language: Punjabi */
	 LANG_GUJARATI = 0x47,               			 					/**< Primary language: Gujarati */
	 LANG_ORIYA = 0x48,                 			 					/**< Primary language: Oriya */
	 LANG_TAMIL = 0x49,                  			 					/**< Primary language: Tamil */
	 LANG_TELUGU = 0x4a,                 			 					/**< Primary language: Telugu */
	 LANG_KANNADA = 0x4b,                			 					/**< Primary language: Kannada */
	 LANG_MALAYALAM = 0x4c,              			 					/**< Primary language: Malayalam */
	 LANG_ASSAMESE = 0x4d,               			 					/**< Primary language: Assamese */
	 LANG_MARATHI = 0x4e,        			 							/**< Primary language: Marathi */
	 LANG_SANSKRIT = 0x4f,       			 							/**< Primary language: Sanskrit */
	 LANG_KONKANI = 0x57,        			 							/**< Primary language: Konkani */
	 LANG_MANIPURI = 0x58,       			 							/**< Primary language: Manipuri */
	 LANG_SINDHI = 0x59,         			 							/**< Primary language: Sindhi */
	 LANG_KASHMIRI = 0x60,       			 							/**< Primary language: Kashmiri */
	 LANG_NEPALI = 0x61,         			 							/**< Primary language: Nepali */
	 LANG_HID = 0xff,			 										/**< Reserved for USB HID class use */
 } USB_PrimLangID_t;
 /*\@}*/

 /** @ingroup USB
  *  USB sublanguage ID codes */
 /*\@{*/
 typedef enum
 {
	 SUBLANG_ARABIC_SAUDI_ARABIA = 0x01,								/**< Sublanguage: Arabic (Saudi Arabia) */
	 SUBLANG_ARABIC_IRAQ = 0x02,										/**< Sublanguage: Arabic (Iraq) */
	 SUBLANG_ARABIC_EGYPT = 0x03,										/**< Sublanguage: Arabic (Egypt) */
	 SUBLANG_ARABIC_LIBYA = 0x04,										/**< Sublanguage: Arabic (Libya) */
	 SUBLANG_ARABIC_ALGERIA = 0x05,										/**< Sublanguage: Arabic (Algeria) */
	 SUBLANG_ARABIC_MOROCCO = 0x06,										/**< Sublanguage: Arabic (Morocco) */
	 SUBLANG_ARABIC_TUNISIA = 0x07,										/**< Sublanguage: Arabic (Tunisia) */
	 SUBLANG_ARABIC_OMAN = 0x08,										/**< Sublanguage: Arabic (Oman) */
	 SUBLANG_ARABIC_YEMEN = 0x09,										/**< Sublanguage: Arabic (Yemen) */
	 SUBLANG_ARABIC_SYRIA = 0x10,										/**< Sublanguage: Arabic (Syria) */
	 SUBLANG_ARABIC_JORDAN = 0x11,										/**< Sublanguage: Arabic (Jordan) */
	 SUBLANG_ARABIC_LEBANON = 0x12,										/**< Sublanguage: Arabic (Lebanon) */
	 SUBLANG_ARABIC_KUWAIT = 0x13,										/**< Sublanguage: Arabic (Kuwait) */
	 SUBLANG_ARABIC_UAE = 0x14,											/**< Sublanguage: Arabic (U.A.E.) */
	 SUBLANG_ARABIC_BAHRAIN = 0x15,										/**< Sublanguage: Arabic (Bahrain) */
	 SUBLANG_ARABIC_QATAR = 0x16,										/**< Sublanguage: Arabic (Qatar) */
	 SUBLANG_AZERI_CYRILLIC = 0x01,										/**< Sublanguage: Azeri (Cyrillic) */
	 SUBLANG_AZERI_LATIN = 0x02,										/**< Sublanguage: Azeri (Latin) */
	 SUBLANG_CHINESE_TRADITIONAL = 0x01,								/**< Sublanguage: Chinese (Traditional) */
	 SUBLANG_CHINESE_SIMPLIFIED = 0x02,									/**< Sublanguage: Chinese (Simplified) */
	 SUBLANG_CHINESE_HONGKONG = 0x03,									/**< Sublanguage: Chinese (Hong Kong SAR, PRC) */
	 SUBLANG_CHINESE_SINGAPORE = 0x04,									/**< Sublanguage: Chinese (Singapore) */
	 SUBLANG_CHINESE_MACAU = 0x05,										/**< Sublanguage: Chinese (Macau SAR) */
	 SUBLANG_DUTCH = 0x01,												/**< Sublanguage: Dutch */
	 SUBLANG_DUTCH_BELGIAN = 0x02,										/**< Sublanguage: Dutch (Belgian) */
	 SUBLANG_ENGLISH_US = 0x01,											/**< Sublanguage: English (US) */
	 SUBLANG_ENGLISH_UK = 0x02,											/**< Sublanguage: English (UK) */
	 SUBLANG_ENGLISH_AUS = 0x03,										/**< Sublanguage: English (Australian) */
	 SUBLANG_ENGLISH_CAN = 0x04,										/**< Sublanguage: English (Canadian) */
	 SUBLANG_ENGLISH_NZ = 0x05,											/**< Sublanguage: English (New Zealand) */
	 SUBLANG_ENGLISH_EIRE = 0x06,										/**< Sublanguage: English (Ireland) */
	 SUBLANG_ENGLISH_SOUTH_AFRICA = 0x07,								/**< Sublanguage: English (South Africa) */
	 SUBLANG_ENGLISH_JAMAICA = 0x08,									/**< Sublanguage: English (Jamaica) */
	 SUBLANG_ENGLISH_CARIBBEAN = 0x09,									/**< Sublanguage: English (Caribbean) */
	 SUBLANG_ENGLISH_BELIZE = 0x0a,										/**< Sublanguage: English (Belize) */
	 SUBLANG_ENGLISH_TRINIDAD = 0x0b,									/**< Sublanguage: English (Trinidad) */
	 SUBLANG_ENGLISH_PHILIPPINES = 0x0c,								/**< Sublanguage: English (Zimbabwe) */
	 SUBLANG_ENGLISH_ZIMBABWE = 0x0d,									/**< Sublanguage: English (Philippines) */
	 SUBLANG_FRENCH = 0x01,												/**< Sublanguage: French */
	 SUBLANG_FRENCH_BELGIAN = 0x02,										/**< Sublanguage: French (Belgian) */
	 SUBLANG_FRENCH_CANADIAN = 0x03,									/**< Sublanguage: French (Canadian) */
	 SUBLANG_FRENCH_SWISS = 0x04,										/**< Sublanguage: French (Swiss) */
	 SUBLANG_FRENCH_LUXEMBOURG = 0x05,									/**< Sublanguage: French (Luxembourg) */
	 SUBLANG_FRENCH_MONACO = 0x06,										/**< Sublanguage: French (Monaco) */
	 SUBLANG_GERMAN = 0x01,												/**< Sublanguage: German */
	 SUBLANG_GERMAN_SWISS = 0x02,										/**< Sublanguage: German (Swiss) */
	 SUBLANG_GERMAN_AUSTRIAN = 0x03,									/**< Sublanguage: German (Austrian) */
	 SUBLANG_GERMAN_LUXEMBOURG = 0x04,									/**< Sublanguage: German (Luxembourg) */
	 SUBLANG_GERMAN_LIECHTENSTEIN = 0x05,								/**< Sublanguage: German (Liechtenstein) */
	 SUBLANG_ITALIAN = 0x01,											/**< Sublanguage: Italian */
	 SUBLANG_ITALIAN_SWISS = 0x02,										/**< Sublanguage: Italian (Swiss) */
	 SUBLANG_KASHMIRI_INDIA = 0x02,										/**< Sublanguage: Kashmiri (India) */
	 SUBLANG_KOREAN = 0x01,												/**< Sublanguage: Korean */
	 SUBLANG_LITHUANIAN = 0x01,											/**< Sublanguage: Lithuanian */
	 SUBLANG_MALAY_MALAYSIA = 0x01,										/**< Sublanguage: Malay (Malaysia) */
	 SUBLANG_MALAY_BRUNEI_DARUSSALAM = 0x02,							/**< Sublanguage: Malay (Brunei Darassalam) */
	 SUBLANG_NEPALI_INDIA = 0x02,										/**< Sublanguage: Nepali (India) */
	 SUBLANG_NORWEGIAN_BOKMAL = 0x01,									/**< Sublanguage: Norwegian (Bokmal) */
	 SUBLANG_NORWEGIAN_NYNORSK = 0x02,									/**< Sublanguage: Norwegian (Nynorsk) */
	 SUBLANG_PORTUGUESE = 0x01,											/**< Sublanguage: Portuguese (Brazilian) */
	 SUBLANG_PORTUGUESE_BRAZILIAN = 0x02,								/**< Sublanguage: Portuguese */
	 SUBLANG_SERBIAN_LATIN = 0x02,										/**< Sublanguage: Serbian (Latin) */
	 SUBLANG_SERBIAN_CYRILLIC = 0x03,									/**< Sublanguage: Serbian (Cyrillic) */
	 SUBLANG_SPANISH = 0x01,											/**< Sublanguage: Spanish (Castilian) */
	 SUBLANG_SPANISH_MEXICAN = 0x02,									/**< Sublanguage: Spanish (Mexican) */
	 SUBLANG_SPANISH_MODERN = 0x03,										/**< Sublanguage: Spanish (Modern) */
	 SUBLANG_SPANISH_GUATEMALA = 0x04,									/**< Sublanguage: Spanish (Guatemala) */
	 SUBLANG_SPANISH_COSTA_RICA = 0x05,									/**< Sublanguage: Spanish (Costa Rica) */
	 SUBLANG_SPANISH_PANAMA = 0x06,										/**< Sublanguage: Spanish (Panama) */
	 SUBLANG_SPANISH_DOMINICAN_REPUBLIC = 0x07, 						/**< Sublanguage: Spanish (Dominican Republic) */
	 SUBLANG_SPANISH_VENEZUELA = 0x08,									/**< Sublanguage: Spanish (Venezuela) */
	 SUBLANG_SPANISH_COLOMBIA = 0x09,									/**< Sublanguage: Spanish (Colombia) */
	 SUBLANG_SPANISH_PERU = 0x0a,										/**< Sublanguage: Spanish (Peru) */
	 SUBLANG_SPANISH_ARGENTINA = 0x0b,									/**< Sublanguage: Spanish (Argentina) */
	 SUBLANG_SPANISH_ECUADOR = 0x0c,									/**< Sublanguage: Spanish (Ecuador) */
	 SUBLANG_SPANISH_CHILE = 0x0d,										/**< Sublanguage: Spanish (Chile) */
	 SUBLANG_SPANISH_URUGUAY = 0x0e,									/**< Sublanguage: Spanish (Uruguay) */
	 SUBLANG_SPANISH_PARAGUAY = 0x0f,									/**< Sublanguage: Spanish (Paraguay) */
	 SUBLANG_SPANISH_BOLIVIA = 0x10,									/**< Sublanguage: Spanish (Bolivia) */
	 SUBLANG_SPANISH_EL_SALVADOR = 0x11,								/**< Sublanguage: Spanish (El Salvador) */
	 SUBLANG_SPANISH_HONDURAS = 0x12,									/**< Sublanguage: Spanish (Honduras) */
	 SUBLANG_SPANISH_NICARAGUA = 0x13,									/**< Sublanguage: Spanish (Nicaragua) */
	 SUBLANG_SPANISH_PUERTO_RICO = 0x14,								/**< Sublanguage: Spanish (Puerto Rico) */
	 SUBLANG_SWEDISH = 0x01,											/**< Sublanguage: Swedish */
	 SUBLANG_SWEDISH_FINLAND = 0x02,									/**< Sublanguage: Swedish (Finland) */
	 SUBLANG_URDU_PAKISTAN = 0x01,										/**< Sublanguage: Urdu (Pakistan) */
	 SUBLANG_URDU_INDIA = 0x02,											/**< Sublanguage: Urdu (India) */
	 SUBLANG_UZBEK_LATIN = 0x01,										/**< Sublanguage: Uzbek (Latin) */
	 SUBLANG_UZBEK_CYRILLIC = 0x02,										/**< Sublanguage: Uzbek (Cyrillic) */
	 SUBLANG_HID_USAGE_DATA_DESCRIPTOR = 0x01,							/**< Sublanguage: HID (Usage Data Descriptor) */
	 SUBLANG_HID_VENDOR_DEFINED_1 = 0x3c,								/**< Sublanguage: HID (Vendor Defined 1) */
	 SUBLANG_HID_VENDOR_DEFINED_2 = 0x3d,								/**< Sublanguage: HID (Vendor Defined 2) */
	 SUBLANG_HID_VENDOR_DEFINED_3 = 0x3e,								/**< Sublanguage: HID (Vendor Defined 3) */
	 SUBLANG_HID_VENDOR_DEFINED_4 = 0x3f,								/**< Sublanguage: HID (Vendor Defined 4) */
 } USB_SubLangID_t;
 /*\@}*/

 /** @ingroup USB
  *  USB subclasses */
 /*\@{*/
 typedef enum
 {
	 USB_SUBCLASS_NONE = 0x00,											/**< No subclass */
	 USB_SUBCLASS_BOOT_INTERFACE = 0x01,								/**< Boot interface subclass */
 } USB_Subclass_t;
 /*\@}*/

 /** @ingroup USB
  *  USB protocols
  *  NOTE: Only needed if you choose #USB_SUBCLASS_BOOT_INTERFACE in the Interface Descriptor. Otherwise set it to #USB_PROTOCOL_NONE. */
 /*\@{*/
 typedef enum
 {
	 USB_PROTOCOL_NONE = 0x00,											/**< No protocol */
	 USB_PROTOCOL_KEYBOARD = 0x01,										/**< Keyboard protocol */
	 USB_PROTOCOL_MOUSE = 0x02,											/**< Mouse protocol */
 } USB_Protocol_t;
 /*\@}*/

 /** @ingroup USB
  *  USB configuration #bmAttributes */
 /*\@{*/
 typedef enum
 {
	 USB_CONFIG_SELF_POWERED = 0x40,									/**< Device Self-powered mask */
	 USB_CONFIG_REMOTE_WAKE = 0x20,										/**< Remote Wakeup attribute mask */
 } USB_ConfigAttributes_t;
 /*\@}*/

 /** @ingroup USB
  *  USB endpoint #bmAttributes */
 /*\@{*/
 typedef enum
 {
	 USB_ENDPOINT_USAGE_DATA = 0x00,									/**< Data endpoint */
	 USB_ENDPOINT_USAGE_FEEDBACK = 0x01,								/**< Feedback endpoint */
	 USB_ENDPOINT_USAGE_IMPL_DATA = 0x02,								/**< Implicit feedback Data endpoint */
	 USB_ENDPOINT_SYNC_NO = 0x00,										/**< No Synchronization */
	 USB_ENDPOINT_SYNC_ASYNC = 0x01,									/**< Asynchronous Synchronization */
	 USB_ENDPOINT_SYNC_ADAPTIVE = 0x02,									/**< Adaptive Synchronization */
	 USB_ENDPOINT_SYNC_SYNCH = 0x03,									/**< Synchronous Synchronization */
	 USB_ENDPOINT_TRANSFER_CONTROL = 0x00,								/**< Control transfer */
	 USB_ENDPOINT_TRANSFER_ISOCHR = 0x01,								/**< Isochronous transfer */
	 USB_ENDPOINT_TRANSFER_BULK = 0x02,									/**< Bulk transfer */
	 USB_ENDPOINT_TRANSFER_INTERRUPT = 0x03,							/**< Interrupt transfer */
 } USB_EndpointAttributes_t;
 /*\@}*/

 /** @ingroup USB
  *  USB device descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< DEVICE descriptor type (=1) */
	 uint16_t bcdUSB;													/**< USB Spec release number */
	 uint8_t bDeviceClass;												/**< Class code assigned by USB-IF */
	 uint8_t bDeviceSubClass;											/**< SubClass Code assigned by USB-IF */
	 uint8_t bDeviceProtocol;											/**< Protocol Code assigned by USB-IF */
	 uint8_t bMaxPacketSize0;											/**< Max packet size for endpoint 0. Must be 8, 16, 32 or 64 */
	 uint16_t idVendor;													/**< Vendor ID - must be obtained from USB-IF */
	 uint16_t idProduct;												/**< Product ID - assigned by the manufacturer */
	 uint16_t bcdDevice;												/**< Device release number in binary coded decimal */
	 uint8_t iManufacturer;												/**< Index of string descriptor describing the manufacturer - set to 0 if no string */
	 uint8_t iProduct;													/**< Index of string descriptor describing the product - set to 0 if no string */
	 uint8_t iSerialNumber;												/**< Index of string descriptor describing the device serial number - set to 0 if no string */
	 uint8_t bNumConfigurations; 										/**< Number of possible configurations */
 } __attribute__((packed)) USB_DeviceDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB configuration descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< CONFIGURATION descriptor type (=2) */
	 uint16_t wTotalLength;												/**< Total number of bytes in this descriptor and all the following descriptors */
	 uint8_t bNumInterfaces;											/**< Number of interfaces supported by this configuration */
	 uint8_t bConfigurationValue;										/**< Value used by Set Configuration to select this configuration */
	 uint8_t iConfiguration;											/**< Index of string descriptor describing configuration - set to 0 if no string */
	 uint8_t bmAttributes;												/**< D7: Must be set to 1
																			 D6: Self-powered
																			 D5: Remote Wakeup
																			 D4-D0: Set to 0 */
	 uint8_t bMaxPower;													/**< Maximum current drawn by device in this configuration. In units of 2mA. So 50 means 100 mA */
 } __attribute__((packed)) USB_ConfigurationDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB string descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< STRING descriptor type (=3) */
	 wchar_t bString[];													/**< UNICODE encoded string */
 } __attribute__((packed)) USB_StringDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB interface descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< INTERFACE descriptor type (=4) */
	 uint8_t bInterfaceNumber;											/**< Number identifying this interface. Zero-based value */
	 uint8_t bAlternateSetting;											/**< Value used to select this alternate setting for this interface */
	 uint8_t bNumEndpoints;												/**< Number of endpoints used by this interface. Doesn't include control endpoint 0 */
	 uint8_t bInterfaceClass;											/**< Class code assigned by USB-IF */
	 uint8_t bInterfaceSubClass;										/**< SubClass Code assigned by USB-IF */
	 uint8_t bInterfaceProtocol;										/**< Protocol Code assigned by USB-IF */
	 uint8_t iInterface;												/**< Index of string descriptor describing interface - set to 0 if no string */
 } __attribute__((packed)) USB_InterfaceDescriptor_t;
 /*\@}*/

 /** @ingroup USB
  *  USB endpoint descriptor definition */
 /*\@{*/
 typedef struct
 {
	 uint8_t bLength;													/**< Size of this descriptor in bytes */
	 uint8_t bDescriptorType;											/**< ENDPOINT descriptor type (=5) */
	 uint8_t bEndpointAddress;											/**< The address of this endpoint within the device.
																			 D7: Direction
																			 0 = OUT, 1 = IN
																			 D6-D4: Set to 0
																			 D3-D0: Endpoint number */
	 uint8_t bmAttributes;												/**< D1:0 Transfer Type
																			 00 = Control
																			 01 = Isochronous
																			 10 = Bulk
																			 11 = Interrupt
																			 The following only apply to isochronous endpoints. Else set to 0.
																			 D3:2 Synchronization Type
																			 00 = No Synchronization
																			 01 = Asynchronous
																			 10 = Adaptive
																			 11 = Synchronous
																			 D5:4 Usage Type
																			 00 = Data endpoint
																			 01 = Feedback endpoint
																			 10 = Implicit feedback Data endpoint
																			 11 = Reserved
																			 D7:6 Reserved
																			 Set to 0 */
	 uint16_t wMaxPacketSize;											/**< Maximum packet size this endpoint can send or receive 
																		     when this configuration is selected  */
	 uint8_t bInterval;													/**< Interval for polling endpoint for data transfers. 
																			 Expressed in frames (ms) for low/full speed or micro frames (125us) for high speed */
 } __attribute__((packed)) USB_EndpointDescriptor_t;
 /*\@}*/

 /** @brief		Function prototype to load an descriptor from the program memory.
  *  @wValue	wValue from USB packet
  *  @wIndex	wIndex from USB packet
  *  @Size		Descriptor length	
  *  @return	Pointer to descriptor address in memory
  */
 extern const void* USB_GetDescriptor(const uint16_t wValue, const uint16_t wIndex, uint16_t* Size);

#endif /* DESCRIPTORTYPES_H_  */ 