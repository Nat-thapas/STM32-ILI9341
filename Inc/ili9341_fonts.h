#ifndef __ILI9341_FONTS_H__
#define __ILI9341_FONTS_H__

#include "stdint.h"
#include "stdlib.h"

/**
 * @brief Glyph definition structure
 * @note The glyph location is based on the bottom-left corner of the glyph.
 */
typedef struct {
    /** Bounding box X offset (right) */
    const int8_t bbX;
    /** Bounding box Y offset (up) */
    const int8_t bbY;
    /** Bounding box width */
    const int8_t bbW;
    /** Bounding box height */
    const int8_t bbH;
    /** Horizontal advance to the next character */
    const int8_t advance;
    /** Pointer to the actual glyph data, the data format is binary bitmap stored as uint8_t array.
     * Order is left-to-right, bottom-to-top. */
    const uint8_t* data;
} ILI9341_GlyphDef;

/**
 * @brief Font definition structure
 */
typedef struct {
    /** Start codepoint (inclusive) */
    const uint_fast8_t startCodepoint;
    /** End codepoint (inclusive) */
    const uint_fast8_t endCodepoint;
    /** Glyphs data */
    const ILI9341_GlyphDef* glyphs;
} ILI9341_FontDef;

extern ILI9341_FontDef ILI9341_Font_Terminus6x12b;
extern ILI9341_FontDef ILI9341_Font_Terminus6x12;
extern ILI9341_FontDef ILI9341_Font_Terminus8x14b;
extern ILI9341_FontDef ILI9341_Font_Terminus8x14;
extern ILI9341_FontDef ILI9341_Font_Terminus8x14v;
extern ILI9341_FontDef ILI9341_Font_Terminus8x16b;
extern ILI9341_FontDef ILI9341_Font_Terminus8x16;
extern ILI9341_FontDef ILI9341_Font_Terminus8x16v;
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

extern ILI9341_FontDef ILI9341_Font_Spleen5x8;
extern ILI9341_FontDef ILI9341_Font_Spleen6x12;
extern ILI9341_FontDef ILI9341_Font_Spleen8x16;
extern ILI9341_FontDef ILI9341_Font_Spleen12x24;
extern ILI9341_FontDef ILI9341_Font_Spleen16x32;
extern ILI9341_FontDef ILI9341_Font_Spleen32x64;

extern ILI9341_FontDef ILI9341_Font_Manop6x14;
extern ILI9341_FontDef ILI9341_Font_Manop7x18;
extern ILI9341_FontDef ILI9341_Font_Manop8x20;

#endif  // __ILI9341_FONTS_H__
