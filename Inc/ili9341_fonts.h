#ifndef __ILI9341_FONTS_H__
#define __ILI9341_FONTS_H__

#include "stdint.h"

/**
 * @brief Font definition structure
 */
typedef struct {
    /** Font width in pixels per glyph */
    const uint16_t width;
    /** Font height in pixels per glyph */
    const uint16_t height;
    /** Number of integers used to store a glyph */
    const uint16_t intsPerGlyph;
    /** Pointer to the actual font data, the data format is binary bitmap stored as uint32_t array */
    const uint32_t* data;
} ILI9341_FontDef;

extern ILI9341_FontDef ILI9341_Font_Spleen5x8;
extern ILI9341_FontDef ILI9341_Font_Spleen6x12;
extern ILI9341_FontDef ILI9341_Font_Spleen8x16;
extern ILI9341_FontDef ILI9341_Font_Spleen12x24;
extern ILI9341_FontDef ILI9341_Font_Spleen16x32;
extern ILI9341_FontDef ILI9341_Font_Spleen32x64;

extern ILI9341_FontDef ILI9341_Font_Terminus6x12b;
extern ILI9341_FontDef ILI9341_Font_Terminus6x12;
extern ILI9341_FontDef ILI9341_Font_Terminus8x14b;
extern ILI9341_FontDef ILI9341_Font_Terminus8x14;
extern ILI9341_FontDef ILI9341_Font_Terminus8x16b;
extern ILI9341_FontDef ILI9341_Font_Terminus8x16;
extern ILI9341_FontDef ILI9341_Font_Terminus10x18b;
extern ILI9341_FontDef ILI9341_Font_Terminus10x18;
extern ILI9341_FontDef ILI9341_Font_Terminus10x20b;
extern ILI9341_FontDef ILI9341_Font_Terminus10x20;
extern ILI9341_FontDef ILI9341_Font_Terminus11x22b;
extern ILI9341_FontDef ILI9341_Font_Terminus11x22;
extern ILI9341_FontDef ILI9341_Font_Terminus12x24b;
extern ILI9341_FontDef ILI9341_Font_Terminus12x24;
extern ILI9341_FontDef ILI9341_Font_Terminus14x28b;
extern ILI9341_FontDef ILI9341_Font_Terminus14x28;
extern ILI9341_FontDef ILI9341_Font_Terminus16x32b;
extern ILI9341_FontDef ILI9341_Font_Terminus16x32;

#endif  // __ILI9341_FONTS_H__
