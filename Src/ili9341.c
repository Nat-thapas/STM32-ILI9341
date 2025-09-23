#include "ili9341.h"

#include "stm32f7xx_hal.h"

/**
 * @brief Select the ILI9341 display
 * @param ili9341 Pointer to ILI9341 handle structure
 */
static void ILI9341_Select(ILI9341_HandleTypeDef* ili9341) {
    HAL_GPIO_WritePin(ili9341->cs_port, ili9341->cs_pin, GPIO_PIN_RESET);
}

void ILI9341_Deselect(ILI9341_HandleTypeDef* ili9341) {
    HAL_GPIO_WritePin(ili9341->cs_port, ili9341->cs_pin, GPIO_PIN_SET);
}

/**
 * @brief Reset the ILI9341 display
 * @param ili9341 Pointer to ILI9341 handle structure
 */
static void ILI9341_Reset(ILI9341_HandleTypeDef* ili9341) {
    HAL_GPIO_WritePin(ili9341->rst_port, ili9341->rst_pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ili9341->rst_port, ili9341->rst_pin, GPIO_PIN_SET);
}

/**
 * @brief Write a command to the ILI9341 display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param cmd Command byte to write
 */
static void ILI9341_WriteCommand(ILI9341_HandleTypeDef* ili9341, uint8_t cmd) {
    HAL_GPIO_WritePin(ili9341->dc_port, ili9341->dc_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(ili9341->spi_handle, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

/**
 * @brief Write data to the ILI9341 display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param buff Pointer to the data buffer
 * @param bufferSize Size of the data buffer
 */
static void ILI9341_WriteData(ILI9341_HandleTypeDef* ili9341, uint8_t* buff, size_t bufferSize) {
    HAL_GPIO_WritePin(ili9341->dc_port, ili9341->dc_pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while (bufferSize > 0) {
        uint16_t chunkSize = bufferSize > 32768 ? 32768 : bufferSize;
        HAL_SPI_Transmit(ili9341->spi_handle, buff, chunkSize, HAL_MAX_DELAY);
        buff += chunkSize;
        bufferSize -= chunkSize;
    }
}

/**
 * @brief Set the address window for subsequent pixel data
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x0 X coordinate of the top-left corner of the window
 * @param y0 Y coordinate of the top-left corner of the window
 * @param x1 X coordinate of the bottom-right corner of the window
 * @param y1 Y coordinate of the bottom-right corner of the window
 */
static void ILI9341_SetAddressWindow(
    ILI9341_HandleTypeDef* ili9341,
    uint16_t x0,
    uint16_t y0,
    uint16_t x1,
    uint16_t y1
) {
    // column address set
    ILI9341_WriteCommand(ili9341, 0x2A);  // CASET
    {
        uint8_t data[] = {(x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(ili9341, 0x2B);  // RASET
    {
        uint8_t data[] = {(y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(ili9341, 0x2C);  // RAMWR
}

ILI9341_HandleTypeDef ILI9341_Init(
    SPI_HandleTypeDef* spi_handle,
    GPIO_TypeDef* cs_port,
    uint16_t cs_pin,
    GPIO_TypeDef* dc_port,
    uint16_t dc_pin,
    GPIO_TypeDef* rst_port,
    uint16_t rst_pin,
    int_fast8_t rotation,
    int_fast16_t width,
    int_fast16_t height
) {
    width = abs(width);
    height = abs(height);

    ILI9341_HandleTypeDef ili9341_instance = {
        .spi_handle = spi_handle,
        .cs_port = cs_port,
        .cs_pin = cs_pin,
        .dc_port = dc_port,
        .dc_pin = dc_pin,
        .rst_port = rst_port,
        .rst_pin = rst_pin,
        .rotation = rotation,
        .width = width,
        .height = height
    };

    ILI9341_HandleTypeDef* ili9341 = &ili9341_instance;

    ILI9341_Select(ili9341);
    ILI9341_Reset(ili9341);

    // Initialization command list is based on https://github.com/martnak/STM32-ILI9341 which have the following license

    /*
        MIT License

        Copyright (c) 2017 martnak

        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"), to deal
        in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
        copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
        SOFTWARE.
    */

    // SOFTWARE RESET
    ILI9341_WriteCommand(ili9341, 0x01);
    HAL_Delay(1000);

    // POWER CONTROL A
    ILI9341_WriteCommand(ili9341, 0xCB);
    {
        uint8_t data[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(ili9341, 0xCF);
    {
        uint8_t data[] = {0x00, 0xC1, 0x30};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(ili9341, 0xE8);
    {
        uint8_t data[] = {0x85, 0x00, 0x78};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(ili9341, 0xEA);
    {
        uint8_t data[] = {0x00, 0x00};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(ili9341, 0xED);
    {
        uint8_t data[] = {0x64, 0x03, 0x12, 0x81};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(ili9341, 0xF7);
    {
        uint8_t data[] = {0x20};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(ili9341, 0xC0);
    {
        uint8_t data[] = {0x23};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(ili9341, 0xC1);
    {
        uint8_t data[] = {0x10};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // VCM CONTROL
    ILI9341_WriteCommand(ili9341, 0xC5);
    {
        uint8_t data[] = {0x3E, 0x28};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(ili9341, 0xC7);
    {
        uint8_t data[] = {0x86};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(ili9341, 0x36);
    {
        uint8_t data[] = {0x48};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(ili9341, 0x3A);
    {
        uint8_t data[] = {0x55};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(ili9341, 0xB1);
    {
        uint8_t data[] = {0x00, 0x18};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(ili9341, 0xB6);
    {
        uint8_t data[] = {0x08, 0x82, 0x27};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(ili9341, 0xF2);
    {
        uint8_t data[] = {0x00};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(ili9341, 0x26);
    {
        uint8_t data[] = {0x01};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(ili9341, 0xE0);
    {
        uint8_t data[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(ili9341, 0xE1);
    {
        uint8_t data[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(ili9341, 0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(ili9341, 0x29);

    // MADCTL
    ILI9341_WriteCommand(ili9341, 0x36);
    switch (ili9341->rotation) {
        case ILI9341_ROTATION_VERTICAL_1: {
            uint8_t rotation = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }
        case ILI9341_ROTATION_HORIZONTAL_1: {
            uint8_t rotation = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }
        case ILI9341_ROTATION_HORIZONTAL_2: {
            uint8_t rotation = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }
        case ILI9341_ROTATION_VERTICAL_2: {
            uint8_t rotation = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }

        default:
            break;
    }

    ILI9341_Deselect(ili9341);

    return ili9341_instance;
}

void ILI9341_SetOrientation(ILI9341_HandleTypeDef* ili9341, int_fast8_t rotation) {
    ILI9341_Select(ili9341);

    // MADCTL
    ILI9341_WriteCommand(ili9341, 0x36);
    switch (rotation) {
        case ILI9341_ROTATION_VERTICAL_1: {
            uint8_t rotation = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }
        case ILI9341_ROTATION_HORIZONTAL_1: {
            uint8_t rotation = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }
        case ILI9341_ROTATION_HORIZONTAL_2: {
            uint8_t rotation = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }
        case ILI9341_ROTATION_VERTICAL_2: {
            uint8_t rotation = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
            uint8_t data[] = {rotation};
            ILI9341_WriteData(ili9341, data, sizeof(data));
            break;
        }

        default:
            break;
    }

    if ((ili9341->rotation == ILI9341_ROTATION_HORIZONTAL_1 || ili9341->rotation == ILI9341_ROTATION_HORIZONTAL_2) &&
        (rotation == ILI9341_ROTATION_VERTICAL_1 || rotation == ILI9341_ROTATION_VERTICAL_2)) {
        int_fast16_t temp = ili9341->width;
        ili9341->width = ili9341->height;
        ili9341->height = temp;
    } else if ((ili9341->rotation == ILI9341_ROTATION_VERTICAL_1 || ili9341->rotation == ILI9341_ROTATION_VERTICAL_2) &&
               (rotation == ILI9341_ROTATION_HORIZONTAL_1 || rotation == ILI9341_ROTATION_HORIZONTAL_2)) {
        int_fast16_t temp = ili9341->width;
        ili9341->width = ili9341->height;
        ili9341->height = temp;
    }

    ili9341->rotation = rotation;

    ILI9341_Deselect(ili9341);
}

void ILI9341_SetBrightness(ILI9341_HandleTypeDef* ili9341, uint_fast8_t brightness) {
    ILI9341_Select(ili9341);

    ILI9341_WriteCommand(ili9341, 0x51);
    {
        uint8_t data[] = {brightness & 0xFF};
        ILI9341_WriteData(ili9341, data, sizeof(data));
    }

    ILI9341_Deselect(ili9341);
}

/**
 * @brief Draw a pixel at specified coordinates without selecting/deselecting the display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the pixel
 * @param y Y coordinate of the pixel
 * @param color 16-bit pixel color in RGB565 format
 */
static void ILI9341_DrawPixelFast(ILI9341_HandleTypeDef* ili9341, int_fast16_t x, int_fast16_t y, uint16_t color) {
    if (x < 0 || y < 0 || x >= ili9341->width || y >= ili9341->height) return;

    ILI9341_SetAddressWindow(ili9341, x, y, x + 1, y + 1);
    uint8_t data[] = {color >> 8, color & 0xFF};
    ILI9341_WriteData(ili9341, data, sizeof(data));
}

void ILI9341_DrawPixel(ILI9341_HandleTypeDef* ili9341, int_fast16_t x, int_fast16_t y, uint16_t color) {
    ILI9341_Select(ili9341);
    ILI9341_DrawPixelFast(ili9341, x, y, color);
    ILI9341_Deselect(ili9341);
}

/**
 * @brief Fill a rectangle without selecting/deselecting the display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left corner of the rectangle
 * @param y Y coordinate of the top-left corner of the rectangle
 * @param w Width of the rectangle in pixels, can be negative
 * @param h Height of the rectangle in pixels, can be negative
 * @param color 16-bit fill color in RGB565 format
 */
static void ILI9341_FillRectangleFast(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    int_fast16_t w,
    int_fast16_t h,
    uint16_t color
) {
    if (w < 0) {
        w = -w;
        x -= w - 1;
    }

    if (h < 0) {
        h = -h;
        y -= h - 1;
    }

    if (x >= ili9341->width || y >= ili9341->height || w == 0 || h == 0) return;

    if (x < 0) {
        w += x;
        x = 0;
        if (w <= 0) return;
    }

    if (y < 0) {
        h += y;
        y = 0;
        if (h <= 0) return;
    }

    if ((x + w - 1) >= ili9341->width) w = ili9341->width - x;
    if ((y + h - 1) >= ili9341->height) h = ili9341->height - y;

    uint8_t buffer[ILI9341_FILL_RECT_BUFFER_SIZE * 2];
    size_t totalSize = w * h;
    size_t chunkSize = totalSize > ILI9341_FILL_RECT_BUFFER_SIZE ? ILI9341_FILL_RECT_BUFFER_SIZE : totalSize;

    color = (color >> 8) | (color << 8);
    for (size_t i = 0; i < chunkSize; i++) { *(uint16_t*)&buffer[i * 2] = color; }

    ILI9341_SetAddressWindow(ili9341, x, y, x + w - 1, y + h - 1);

    while (totalSize > 0) {
        ILI9341_WriteData(ili9341, buffer, chunkSize * 2);
        totalSize -= chunkSize;
        chunkSize = (totalSize > ILI9341_FILL_RECT_BUFFER_SIZE) ? ILI9341_FILL_RECT_BUFFER_SIZE : totalSize;
    }
}

void ILI9341_FillRectangle(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    int_fast16_t w,
    int_fast16_t h,
    uint16_t color
) {
    ILI9341_Select(ili9341);
    ILI9341_FillRectangleFast(ili9341, x, y, w, h, color);
    ILI9341_Deselect(ili9341);
}

void ILI9341_FillScreen(ILI9341_HandleTypeDef* ili9341, uint16_t color) {
    ILI9341_Select(ili9341);
    ILI9341_FillRectangleFast(ili9341, 0, 0, ili9341->width, ili9341->height, color);
    ILI9341_Deselect(ili9341);
}

/**
 * @brief Write a character at specified coordinates without selecting/deselecting the display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left of the character
 * @param y Y coordinate of the top-left of the character
 * @param ch ASCII character to write
 * @param font Font definition to use for rendering the character
 * @param color 16-bit character color in RGB565 format
 * @param bgColor 16-bit background color in RGB565 format
 */
static void ILI9341_WriteChar(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    char ch,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t bgColor
) {
    int_fast16_t endX = x + font.width - 1;
    int_fast16_t endY = y + font.height - 1;

    if (endX < 0 || endY < 0 || x >= ili9341->width || y >= ili9341->height) return;

    int_fast16_t clipStartX = x < 0 ? -x : 0;
    int_fast16_t clipStartY = y < 0 ? -y : 0;
    int_fast16_t clipEndX = endX >= ili9341->width ? ili9341->width - x - 1 : font.width - 1;
    int_fast16_t clipEndY = endY >= ili9341->height ? ili9341->height - y - 1 : font.height - 1;

    if (ch < 32 || ch > 126) ch = 32;

    color = (color >> 8) | (color << 8);
    bgColor = (bgColor >> 8) | (bgColor << 8);

    uint16_t buffer[ILI9341_WRITE_CHAR_BUFFER_SIZE];
    size_t bufferIndex = 0;

    int_fast16_t index = (ch - 32) * font.intsPerGlyph;
    uint32_t mask = 0x80000000;

    ILI9341_SetAddressWindow(ili9341, x + clipStartX, y + clipStartY, x + clipEndX, y + clipEndY);

    for (int_fast16_t row = 0; row < font.height; row++) {
        for (int_fast16_t col = 0; col < font.width; col++) {
            if (row >= clipStartY && row <= clipEndY && col >= clipStartX && col <= clipEndX) {
                if (font.data[index] & mask) {
                    buffer[bufferIndex++] = color;
                } else {
                    buffer[bufferIndex++] = bgColor;
                }

                if (bufferIndex >= ILI9341_WRITE_CHAR_BUFFER_SIZE) {
                    ILI9341_WriteData(ili9341, (uint8_t*)buffer, bufferIndex * 2);
                    bufferIndex = 0;
                }
            }

            mask >>= 1;
            if (mask == 0) {
                index++;
                mask = 0x80000000;
            }
        }
    }

    if (bufferIndex > 0) ILI9341_WriteData(ili9341, (uint8_t*)buffer, bufferIndex * 2);
}

void ILI9341_WriteString(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t bgColor,
    int_fast16_t tracking
) {
    if (y >= ili9341->height) return;

    ILI9341_Select(ili9341);

    while (*str && x < ili9341->width) {
        ILI9341_WriteChar(ili9341, x, y, *str, font, color, bgColor);
        x += font.width + tracking;
        str++;
    }

    ILI9341_Deselect(ili9341);
}

/**
 * @brief Write a scaled character at specified coordinates without selecting/deselecting the display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left of the character
 * @param y Y coordinate of the top-left of the character
 * @param ch ASCII character to write
 * @param font Font definition to use for rendering the character
 * @param color 16-bit character color in RGB565 format
 * @param bgColor 16-bit background color in RGB565 format
 * @param scale Scaling factor (integer) to enlarge the character
 */
static void ILI9341_WriteCharScaled(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    char ch,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t bgColor,
    int_fast16_t scale
) {
    int_fast16_t endX = x + font.width * scale - 1;
    int_fast16_t endY = y + font.height * scale - 1;

    if (endX < 0 || endY < 0 || x >= ili9341->width || y >= ili9341->height) return;

    int_fast16_t clipStartX = x < 0 ? -x : 0;
    int_fast16_t clipStartY = y < 0 ? -y : 0;
    int_fast16_t clipEndX = endX >= ili9341->width ? ili9341->width - x - 1 : font.width * scale - 1;
    int_fast16_t clipEndY = endY >= ili9341->height ? ili9341->height - y - 1 : font.height * scale - 1;

    if (ch < 32 || ch > 126) ch = 32;

    color = (color >> 8) | (color << 8);
    bgColor = (bgColor >> 8) | (bgColor << 8);

    uint16_t buffer[ILI9341_WRITE_CHAR_BUFFER_SIZE];
    size_t bufferIndex = 0;

    int_fast32_t bitIndex = (ch - 32) * font.intsPerGlyph * 32;

    ILI9341_SetAddressWindow(ili9341, x + clipStartX, y + clipStartY, x + clipEndX, y + clipEndY);

    for (int_fast16_t row = 0; row < font.height; row++) {
        for (int_fast16_t vScale = 0; vScale < scale; vScale++) {
            int_fast16_t pixelRow = row * scale + vScale;
            for (int_fast16_t col = 0; col < font.width; col++) {
                uint32_t mask = 0x80000000 >> (bitIndex % 32);
                int_fast16_t index = bitIndex / 32;
                for (int_fast16_t hScale = 0; hScale < scale; hScale++) {
                    int_fast16_t pixelCol = col * scale + hScale;
                    if (pixelRow >= clipStartY && pixelRow <= clipEndY && pixelCol >= clipStartX &&
                        pixelCol <= clipEndX) {
                        if (font.data[index] & mask) {
                            buffer[bufferIndex++] = color;
                        } else {
                            buffer[bufferIndex++] = bgColor;
                        }

                        if (bufferIndex >= ILI9341_WRITE_CHAR_BUFFER_SIZE) {
                            ILI9341_WriteData(ili9341, (uint8_t*)buffer, bufferIndex * 2);
                            bufferIndex = 0;
                        }
                    }
                }
                bitIndex++;
            }
            bitIndex -= font.width;
        }
        bitIndex += font.width;
    }

    if (bufferIndex > 0) ILI9341_WriteData(ili9341, (uint8_t*)buffer, bufferIndex * 2);
}

void ILI9341_WriteStringScaled(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    uint16_t bgColor,
    int_fast16_t scale,
    int_fast16_t tracking
) {
    if (y >= ili9341->height) return;
    scale = abs(scale);

    ILI9341_Select(ili9341);

    while (*str && x < ili9341->width) {
        ILI9341_WriteCharScaled(ili9341, x, y, *str, font, color, bgColor, scale);
        x += font.width * scale + tracking;
        str++;
    }

    ILI9341_Deselect(ili9341);
}

/**
 * @brief Write a character with transparent background at specified coordinates without selecting/deselecting the
 * display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left of the character
 * @param y Y coordinate of the top-left of the character
 * @param ch ASCII character to write
 * @param font Font definition to use for rendering the character
 * @param color 16-bit character color in RGB565 format
 */
static void ILI9341_WriteCharTransparent(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    char ch,
    ILI9341_FontDef font,
    uint16_t color
) {
    if (ch < 32 || ch > 126) ch = 32;

    int_fast16_t index = (ch - 32) * font.intsPerGlyph;
    uint32_t mask = 0x80000000;

    for (int_fast16_t row = 0; row < font.height; row++) {
        for (int_fast16_t col = 0; col < font.width; col++) {
            if (font.data[index] & mask) { ILI9341_DrawPixelFast(ili9341, x + col, y + row, color); }
            mask >>= 1;
            if (mask == 0) {
                index++;
                mask = 0x80000000;
            }
        }
    }
}

void ILI9341_WriteStringTransparent(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    int_fast16_t tracking
) {
    if (y >= ili9341->height) return;

    ILI9341_Select(ili9341);

    while (*str && x < ili9341->width) {
        ILI9341_WriteCharTransparent(ili9341, x, y, *str, font, color);
        x += font.width + tracking;
        str++;
    }

    ILI9341_Deselect(ili9341);
}

/**
 * @brief Write a scaled character with transparent background at specified coordinates without selecting/deselecting
 * the display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x X coordinate of the top-left of the character
 * @param y Y coordinate of the top-left of the character
 * @param ch ASCII character to write
 * @param font Font definition to use for rendering the character
 * @param color 16-bit character color in RGB565 format
 * @param scale Scaling factor (integer) to enlarge the character
 */
static void ILI9341_WriteCharTransparentScaled(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    char ch,
    ILI9341_FontDef font,
    uint16_t color,
    int_fast16_t scale
) {
    if (ch < 32 || ch > 126) ch = 32;

    int_fast16_t index = (ch - 32) * font.intsPerGlyph;
    uint32_t mask = 0x80000000;

    for (int_fast16_t row = 0; row < font.height; row++) {
        for (int_fast16_t col = 0; col < font.width; col++) {
            if (font.data[index] & mask) {
                ILI9341_FillRectangleFast(ili9341, x + col * scale, y + row * scale, scale, scale, color);
            }
            mask >>= 1;
            if (mask == 0) {
                index++;
                mask = 0x80000000;
            }
        }
    }
}

void ILI9341_WriteStringTransparentScaled(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    const char* str,
    ILI9341_FontDef font,
    uint16_t color,
    int_fast16_t scale,
    int_fast16_t tracking
) {
    if (y >= ili9341->height) return;
    scale = abs(scale);

    ILI9341_Select(ili9341);

    while (*str && x < ili9341->width) {
        ILI9341_WriteCharTransparentScaled(ili9341, x, y, *str, font, color, scale);
        x += font.width * scale + tracking;
        str++;
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawImage(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    int_fast16_t w,
    int_fast16_t h,
    const uint16_t* data
) {
    if (w == 0 || h == 0) return;
    if (w < 0) {
        w = -w;
        x -= w - 1;
    }
    if (h < 0) {
        h = -h;
        y -= h - 1;
    }
    if (x < 0 || y < 0 || x >= ili9341->width || y >= ili9341->height || (x + w - 1) >= ili9341->width ||
        (y + h - 1) >= ili9341->height)
        return;

    ILI9341_Select(ili9341);
    ILI9341_SetAddressWindow(ili9341, x, y, x + w - 1, y + h - 1);
    ILI9341_WriteData(ili9341, (uint8_t*)data, sizeof(uint16_t) * w * h);
    ILI9341_Deselect(ili9341);
}

void ILI9341_InvertColors(ILI9341_HandleTypeDef* ili9341, bool invert) {
    ILI9341_Select(ili9341);
    ILI9341_WriteCommand(ili9341, invert ? 0x21 /* INVON */ : 0x20 /* INVOFF */);
    ILI9341_Deselect(ili9341);
}

/**
 * @brief Draw a line using Bresenham's algorithm without selecting/deselecting the display
 * @param ili9341 Pointer to ILI9341 handle structure
 * @param x1 X coordinate of the start point
 * @param y1 Y coordinate of the start point
 * @param x2 X coordinate of the end point
 * @param y2 Y coordinate of the end point
 * @param color 16-bit line color in RGB565 format
 */
static void ILI9341_DrawLineFast(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x1,
    int_fast16_t y1,
    int_fast16_t x2,
    int_fast16_t y2,
    uint16_t color
) {
    if (x1 == x2) {
        ILI9341_FillRectangleFast(ili9341, x1, y1, 1, y2 - y1 + 1, color);
        return;
    } else if (y1 == y2) {
        ILI9341_FillRectangleFast(ili9341, x1, y1, x2 - x1 + 1, 1, color);
        return;
    }

    int_fast16_t dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int_fast16_t dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int_fast16_t err = dx + dy, e2; /* error value e_xy */

    while (true) {
        ILI9341_DrawPixelFast(ili9341, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            err += dx;
            y1 += sy;
        }
    }
}

void ILI9341_DrawLine(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x1,
    int_fast16_t y1,
    int_fast16_t x2,
    int_fast16_t y2,
    uint16_t color
) {
    ILI9341_Select(ili9341);
    ILI9341_DrawLineFast(ili9341, x1, y1, x2, y2, color);
    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawLineThick(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x1,
    int_fast16_t y1,
    int_fast16_t x2,
    int_fast16_t y2,
    uint16_t color,
    int_fast16_t thickness,
    bool cap
) {
    if (thickness == 0) return;
    thickness = abs(thickness);
    if (thickness == 1) {
        ILI9341_DrawLine(ili9341, x1, y1, x2, y2, color);
        return;
    }

    // calculate line vector
    int_fast16_t dx = x2 - x1;
    int_fast16_t dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) return;  // zero-length line

    // unit vector
    float ux = dx / length;
    float uy = dy / length;

    // perpendicular vector
    float px = -uy;
    float py = ux;

    // half thickness
    float halfThickness = thickness / 2.0f;

    // calculate corner points
    int16_t xCorners[4] = {
        (int16_t)(x1 + px * halfThickness),
        (int16_t)(x1 - px * halfThickness),
        (int16_t)(x2 - px * halfThickness),
        (int16_t)(x2 + px * halfThickness)
    };
    int16_t yCorners[4] = {
        (int16_t)(y1 + py * halfThickness),
        (int16_t)(y1 - py * halfThickness),
        (int16_t)(y2 - py * halfThickness),
        (int16_t)(y2 + py * halfThickness)
    };

    ILI9341_FillPolygon(ili9341, xCorners, yCorners, 4, color);

    if (cap) {
        ILI9341_FillCircle(ili9341, x1, y1, halfThickness, color);
        ILI9341_FillCircle(ili9341, x2, y2, halfThickness, color);
    }
}

void ILI9341_DrawRectangle(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    int_fast16_t w,
    int_fast16_t h,
    uint16_t color
) {
    ILI9341_DrawRectangleThick(ili9341, x, y, w, h, color, 1);
}

void ILI9341_DrawRectangleThick(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t x,
    int_fast16_t y,
    int_fast16_t w,
    int_fast16_t h,
    uint16_t color,
    int_fast16_t thickness
) {
    if (thickness == 0) return;
    thickness = abs(thickness);

    ILI9341_Select(ili9341);

    ILI9341_FillRectangleFast(ili9341, x, y, w, thickness, color);                              // top
    ILI9341_FillRectangleFast(ili9341, x, y + h - thickness, w, thickness, color);              // bottom
    ILI9341_FillRectangleFast(ili9341, x, y + thickness, thickness, h - 2 * thickness, color);  // left
    ILI9341_FillRectangleFast(ili9341, x + w - thickness, y + thickness, thickness, h - 2 * thickness, color);  // right

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawCircle(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t xc,
    int_fast16_t yc,
    int_fast16_t r,
    uint16_t color
) {
    r = abs(r);
    if (r == 0 || xc + r < 0 || xc - r >= ili9341->width || yc + r < 0 || yc - r >= ili9341->height) return;

    int_fast16_t f = 1 - r;
    int_fast16_t ddF_x = 1;
    int_fast16_t ddF_y = -2 * r;
    int_fast16_t x = 0;
    int_fast16_t y = r;

    ILI9341_Select(ili9341);

    ILI9341_DrawPixelFast(ili9341, xc, yc + r, color);
    ILI9341_DrawPixelFast(ili9341, xc, yc - r, color);
    ILI9341_DrawPixelFast(ili9341, xc + r, yc, color);
    ILI9341_DrawPixelFast(ili9341, xc - r, yc, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        ILI9341_DrawPixelFast(ili9341, xc + x, yc + y, color);
        ILI9341_DrawPixelFast(ili9341, xc - x, yc + y, color);
        ILI9341_DrawPixelFast(ili9341, xc + x, yc - y, color);
        ILI9341_DrawPixelFast(ili9341, xc - x, yc - y, color);
        ILI9341_DrawPixelFast(ili9341, xc + y, yc + x, color);
        ILI9341_DrawPixelFast(ili9341, xc - y, yc + x, color);
        ILI9341_DrawPixelFast(ili9341, xc + y, yc - x, color);
        ILI9341_DrawPixelFast(ili9341, xc - y, yc - x, color);
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawCircleThick(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t xc,
    int_fast16_t yc,
    int_fast16_t r,
    uint16_t color,
    int_fast16_t thickness
) {
    r = abs(r);
    if (r == 0 || thickness == 0 || xc + r < 0 || xc - r >= ili9341->width || yc + r < 0 || yc - r >= ili9341->height)
        return;
    thickness = abs(thickness);
    if (thickness > r) thickness = r;
    if (thickness == 1) {
        ILI9341_DrawCircle(ili9341, xc, yc, r, color);
        return;
    }

    int_fast16_t ri = r - thickness;
    int_fast16_t xo = r;
    int_fast16_t xi = ri;

    ILI9341_Select(ili9341);

    for (int_fast16_t y = 0; y <= r; y++) {
        while (xo * xo + y * y > r * r) { xo--; }
        while (xi * xi + y * y > ri * ri) { xi--; }

        ILI9341_DrawLineFast(ili9341, xc - xo, yc + y, xc - xi, yc + y, color);
        ILI9341_DrawLineFast(ili9341, xc + xi, yc + y, xc + xo, yc + y, color);
        ILI9341_DrawLineFast(ili9341, xc - xo, yc - y, xc - xi, yc - y, color);
        ILI9341_DrawLineFast(ili9341, xc + xi, yc - y, xc + xo, yc - y, color);
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_FillCircle(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t xc,
    int_fast16_t yc,
    int_fast16_t r,
    uint16_t color
) {
    r = abs(r);
    if (r == 0 || xc + r < 0 || xc - r >= ili9341->width || yc + r < 0 || yc - r >= ili9341->height) return;

    int_fast16_t x = r;

    ILI9341_Select(ili9341);

    for (int_fast16_t y = 0; y <= r; y++) {
        while (x * x + y * y > r * r) { x--; }
        ILI9341_DrawLineFast(ili9341, xc - x, yc + y, xc + x, yc + y, color);
        ILI9341_DrawLineFast(ili9341, xc - x, yc - y, xc + x, yc - y, color);
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawEllipse(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t xc,
    int_fast16_t yc,
    int_fast16_t rx,
    int_fast16_t ry,
    uint16_t color
) {
    rx = abs(rx);
    ry = abs(ry);
    if (rx == 0 || ry == 0 || xc + rx < 0 || xc - rx >= ili9341->width || yc + ry < 0 || yc - ry >= ili9341->height)
        return;

    int_fast32_t rx2 = rx * rx;
    int_fast32_t ry2 = ry * ry;
    int_fast32_t twoRx2 = 2 * rx2;
    int_fast32_t twoRy2 = 2 * ry2;
    int_fast32_t p;
    int_fast32_t x = 0;
    int_fast32_t y = ry;
    int_fast32_t px = 0;
    int_fast32_t py = twoRx2 * y;

    ILI9341_Select(ili9341);

    // Plot initial points
    ILI9341_DrawPixelFast(ili9341, xc + x, yc + y, color);
    ILI9341_DrawPixelFast(ili9341, xc - x, yc + y, color);
    ILI9341_DrawPixelFast(ili9341, xc + x, yc - y, color);
    ILI9341_DrawPixelFast(ili9341, xc - x, yc - y, color);

    // Region 1
    p = ry2 - (rx2 * ry) + (rx2 / 4);
    while (px < py) {
        x++;
        px += twoRy2;
        if (p < 0) {
            p += ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            p += ry2 + px - py;
        }

        ILI9341_DrawPixelFast(ili9341, xc + x, yc + y, color);
        ILI9341_DrawPixelFast(ili9341, xc - x, yc + y, color);
        ILI9341_DrawPixelFast(ili9341, xc + x, yc - y, color);
        ILI9341_DrawPixelFast(ili9341, xc - x, yc - y, color);
    }

    // Region 2
    // p = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    p = ry2 * (x * x + x) + ry2 / 4 + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p > 0) {
            p += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            p += rx2 - py + px;
        }

        ILI9341_DrawPixelFast(ili9341, xc + x, yc + y, color);
        ILI9341_DrawPixelFast(ili9341, xc - x, yc + y, color);
        ILI9341_DrawPixelFast(ili9341, xc + x, yc - y, color);
        ILI9341_DrawPixelFast(ili9341, xc - x, yc - y, color);
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawEllipseThick(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t xc,
    int_fast16_t yc,
    int_fast16_t rx,
    int_fast16_t ry,
    uint16_t color,
    int_fast16_t thickness
) {
    rx = abs(rx);
    ry = abs(ry);
    if (rx == 0 || ry == 0 || thickness == 0 || xc + rx < 0 || xc - rx >= ili9341->width || yc + ry < 0 ||
        yc - ry >= ili9341->height)
        return;
    thickness = abs(thickness);
    if (thickness > rx) thickness = rx;
    if (thickness > ry) thickness = ry;
    if (thickness == 1) {
        ILI9341_DrawEllipse(ili9341, xc, yc, rx, ry, color);
        return;
    }

    int_fast32_t ai = rx - thickness;
    int_fast32_t bi = ry - thickness;
    int_fast32_t xo = rx;
    int_fast32_t xi = ai;

    ILI9341_Select(ili9341);

    for (int_fast16_t y = 0; y <= ry; y++) {
        while (xo * xo * bi * bi + y * y * ai * ai > rx * rx * ry * ry) { xo--; }
        while (xi * xi * bi * bi + y * y * ai * ai > ai * ai * bi * bi) { xi--; }

        ILI9341_DrawLineFast(ili9341, xc - xo, yc + y, xc - xi, yc + y, color);
        ILI9341_DrawLineFast(ili9341, xc + xi, yc + y, xc + xo, yc + y, color);
        ILI9341_DrawLineFast(ili9341, xc - xo, yc - y, xc - xi, yc - y, color);
        ILI9341_DrawLineFast(ili9341, xc + xi, yc - y, xc + xo, yc - y, color);
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_FillEllipse(
    ILI9341_HandleTypeDef* ili9341,
    int_fast16_t xc,
    int_fast16_t yc,
    int_fast16_t rx,
    int_fast16_t ry,
    uint16_t color
) {
    rx = abs(rx);
    ry = abs(ry);
    if (rx == 0 || ry == 0 || xc + rx < 0 || xc - rx >= ili9341->width || yc + ry < 0 || yc - ry >= ili9341->height)
        return;

    int_fast16_t x = rx;

    ILI9341_Select(ili9341);

    for (int_fast16_t y = 0; y <= ry; y++) {
        while (x * x * ry * ry + y * y * rx * rx > rx * rx * ry * ry) { x--; }
        ILI9341_DrawLineFast(ili9341, xc - x, yc + y, xc + x, yc + y, color);
        ILI9341_DrawLineFast(ili9341, xc - x, yc - y, xc + x, yc - y, color);
    }

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawPolygon(ILI9341_HandleTypeDef* ili9341, int16_t* x, int16_t* y, size_t n, uint16_t color) {
    if (n < 2) return;

    ILI9341_Select(ili9341);

    for (size_t i = 0; i < n - 1; i++) { ILI9341_DrawLineFast(ili9341, x[i], y[i], x[i + 1], y[i + 1], color); }
    ILI9341_DrawLineFast(ili9341, x[n - 1], y[n - 1], x[0], y[0], color);

    ILI9341_Deselect(ili9341);
}

void ILI9341_DrawPolygonThick(
    ILI9341_HandleTypeDef* ili9341,
    int16_t* x,
    int16_t* y,
    size_t n,
    uint16_t color,
    int_fast16_t thickness,
    bool cap
) {
    if (n < 2 || thickness == 0) return;
    thickness = abs(thickness);
    if (thickness == 1) {
        ILI9341_DrawPolygon(ili9341, x, y, n, color);
        return;
    }

    for (size_t i = 0; i < n - 1; i++) {
        ILI9341_DrawLineThick(ili9341, x[i], y[i], x[i + 1], y[i + 1], color, thickness, cap);
    }
    ILI9341_DrawLineThick(ili9341, x[n - 1], y[n - 1], x[0], y[0], color, thickness, cap);
}

void ILI9341_FillPolygon(ILI9341_HandleTypeDef* ili9341, int16_t* x, int16_t* y, size_t n, uint16_t color) {
    if (n < 3) return;

    // find max and min Y
    int_fast16_t minY = y[0], maxY = y[0];
    for (size_t i = 1; i < n; i++) {
        if (y[i] < minY) minY = y[i];
        if (y[i] > maxY) maxY = y[i];
    }

    if (minY >= ili9341->height) return;
    if (maxY < 0) return;
    if (minY < 0) minY = 0;
    if (maxY >= ili9341->height) maxY = ili9341->height - 1;

    int_fast16_t nodeX[32];  // max 32 intersections

    ILI9341_Select(ili9341);

    // scanline algorithm
    for (int_fast16_t j = minY; j <= maxY; j++) {
        int_fast16_t nodes = 0;
        int_fast16_t k = n - 1;

        // find intersections
        for (int_fast16_t i = 0; i < n; i++) {
            if ((y[i] < j && y[k] >= j) || (y[k] < j && y[i] >= j)) {
                int_fast16_t dy = y[k] - y[i];
                if (dy != 0 && nodes < 32) {
                    nodeX[nodes++] = x[i] + ((int_fast32_t)(j - y[i]) * (int_fast32_t)(x[k] - x[i])) / dy;
                }
            }
            k = i;
        }

        // insertion sort nodeX array
        for (int_fast16_t i = 1; i < nodes; i++) {
            int_fast16_t key = nodeX[i];
            int_fast16_t jSort = i - 1;
            while (jSort >= 0 && nodeX[jSort] > key) {
                nodeX[jSort + 1] = nodeX[jSort];
                jSort--;
            }
            nodeX[jSort + 1] = key;
        }

        // fill the pixels between node pairs
        for (int_fast16_t i = 0; i < nodes - 1; i += 2) {
            int_fast16_t x1 = nodeX[i];
            int_fast16_t x2 = nodeX[i + 1];

            if (x1 >= ili9341->width) break;
            if (x2 >= ili9341->width) x2 = ili9341->width - 1;
            if (x1 < ili9341->width && x2 > 0 && x2 >= x1) {
                ILI9341_FillRectangleFast(ili9341, x1, j, x2 - x1 + 1, 1, color);
            }
        }
    }

    ILI9341_Deselect(ili9341);
}
