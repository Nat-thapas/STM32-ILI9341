/**
 * @file    example.c
 * @brief   ILI9341 example and test file
 * @note    This file contains an example code for how to use the ILI9341 library.
 */

#include "ili9341.h"
#include "ili9341_fonts.h"
#include "ili9341_touch.h"

// Image array is generated with image_to_array.py
#include "image.h"

int main(void) {
    ILI9341_HandleTypeDef ili9341 = ILI9341_Init(
        &hspi5,
        ILI9341_CS_GPIO_Port,
        ILI9341_CS_Pin,
        ILI9341_DC_GPIO_Port,
        ILI9341_DC_Pin,
        ILI9341_RST_GPIO_Port,
        ILI9341_RST_Pin,
        ILI9341_ROTATION_HORIZONTAL_1,
        320,
        240
    );

    ILI9341_Touch_HandleTypeDef ili9341_touch = ILI9341_Touch_Init(
        &hspi4,
        ILI9341_Touch_CS_GPIO_Port,
        ILI9341_Touch_CS_Pin,
        ILI9341_Touch_IRQ_GPIO_Port,
        ILI9341_Touch_IRQ_Pin,
        ILI9341_ROTATION_HORIZONTAL_1,
        320,
        240
    );

    while (1) {
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            5,
            31,
            "Welcome to the STM32 ILI9341 Test Suite.",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            5,
            47,
            "Press B1 to continue",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Fill screen test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_RED);
        HAL_Delay(250);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_GREEN);
        HAL_Delay(250);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_BLUE);
        HAL_Delay(250);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_BLACK);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Fill rectangle test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_FillRectangle(
                &ili9341,
                10 + (i * 15),
                30 + (i * 15),
                50,
                50,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Write string fonts test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_WriteString(
            &ili9341,
            10,
            35,
            "Terminus8x16",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            55,
            "Terminus12x24",
            ILI9341_Font_Terminus12x24,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            80,
            "Terminus12x24b",
            ILI9341_Font_Terminus12x24b,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            115,
            "Terminus16x32",
            ILI9341_Font_Terminus16x32,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            150,
            "Terminus16x32b",
            ILI9341_Font_Terminus16x32b,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            165,
            "Spleen5x8",
            ILI9341_Font_Spleen5x8,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            185,
            "Spleen8x16",
            ILI9341_Font_Spleen8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            220,
            "Spleen16x32",
            ILI9341_Font_Spleen16x32,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Write string codepoints test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int i = 1; i < 8; i++) {
            char str[33];
            for (int j = i * 32; j < (i * 32) + 32; j++) { str[j - (i * 32)] = (char)j; }
            str[32] = '\0';
            ILI9341_WriteString(
                &ili9341,
                10,
                35 + (i * 16),
                str,
                ILI9341_Font_Terminus8x16,
                ILI9341_COLOR_BLACK,
                ILI9341_COLOR_WHITE,
                false,
                1,
                0,
                0
            );
        }
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        for (int i = 1; i < 8; i++) {
            char str[33];
            for (int j = i * 32; j < (i * 32) + 32; j++) { str[j - (i * 32)] = (char)j; }
            str[32] = '\0';
            ILI9341_WriteString(
                &ili9341,
                10,
                35 + (i * 20),
                str,
                ILI9341_Font_Manop8x20,
                ILI9341_COLOR_BLACK,
                ILI9341_COLOR_WHITE,
                false,
                1,
                0,
                0
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Write string scaled test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_WriteString(
            &ili9341,
            10,
            35,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            70,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            2,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            120,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            3,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            40,
            245,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            3,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Write string newline / wrap test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_WriteString(
            &ili9341,
            10,
            35,
            "Hello, World!\nThis is a test of the newline character.\nIt should move to the next line.",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            100,
            "Hello, World! This is a test of the wrap feature. It should automatically move to the next line when the "
            "text "
            "exceeds the width of the display.",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            true,
            1,
            0,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            200,
            "Hello, World! There's not a lot of space here."
            "exceeds the width of the display.",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            true,
            3,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Write string tracking, leading test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_WriteString(
            &ili9341,
            10,
            35,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            8,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            60,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            -2,
            0
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            95,
            "Hello, World!\nThis a test of leading in case of multiple lines.",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            8
        );
        ILI9341_WriteString(
            &ili9341,
            10,
            160,
            "Hello, World! This a test of leading in case of multiple lines from automatic line wraps.",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            true,
            1,
            0,
            -4
        );
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Write string transparent test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (uint16_t x = 0; x < ili9341.width; x += 16) {
            for (uint16_t y = 0; y < ili9341.height; y += 16) {
                ILI9341_FillRectangle(&ili9341, x, y, 16, 16, HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF);
            }
        }
        HAL_Delay(250);
        ILI9341_WriteStringTransparent(
            &ili9341,
            10,
            40,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        ILI9341_WriteStringTransparent(
            &ili9341,
            10,
            70,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_WHITE,
            false,
            2,
            0,
            0
        );
        ILI9341_WriteStringTransparent(
            &ili9341,
            10,
            120,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_WHITE,
            false,
            3,
            0,
            0
        );
        ILI9341_WriteStringTransparent(
            &ili9341,
            40,
            245,
            "Hello, World!",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_WHITE,
            false,
            3,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();

        // ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        // ILI9341_WriteString(
        //     &ili9341,
        //     5,
        //     15,
        //     "Draw image test",
        //     ILI9341_Font_Terminus8x16,
        //     ILI9341_COLOR_BLACK,
        //     ILI9341_COLOR_WHITE,
        //     0
        // );
        // HAL_Delay(250);
        // waitForButtonPress();
        // ILI9341_DrawImage(&ili9341, 20, 25, 280, 210, image_data);
        // HAL_Delay(250);
        // waitForButtonPress();

        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Inverted colors test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_InvertColors(&ili9341, true);
        HAL_Delay(250);
        waitForButtonPress();
        ILI9341_InvertColors(&ili9341, false);
        HAL_Delay(250);

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw line test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int i = 0; i < 20; i++) {
            ILI9341_DrawLine(
                &ili9341,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.width * 2) - ili9341.width / 2,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.height * 2) - ili9341.height / 2,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.width * 2) - ili9341.width / 2,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.height * 2) - ili9341.height / 2,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw line thick test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int i = 0; i < 20; i++) {
            ILI9341_DrawLineThick(
                &ili9341,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.width * 2) - ili9341.width / 2,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.height * 2) - ili9341.height / 2,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.width * 2) - ili9341.width / 2,
                HAL_RNG_GetRandomNumber(&hrng) % (ili9341.height * 2) - ili9341.height / 2,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF,
                1 + (HAL_RNG_GetRandomNumber(&hrng) % 16),
                HAL_RNG_GetRandomNumber(&hrng) & 1
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw rectangle test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_DrawRectangle(
                &ili9341,
                10 + (i * 15),
                30 + (i * 15),
                50,
                50,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw rectangle thick test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_DrawRectangleThick(
                &ili9341,
                10 + (i * 15),
                30 + (i * 15),
                50,
                50,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF,
                1 + (HAL_RNG_GetRandomNumber(&hrng) % 16)
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw circle test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_DrawCircle(&ili9341, 25 + (i * 15), 55 + (i * 15), 25, HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF);
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw circle thick test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_DrawCircleThick(
                &ili9341,
                25 + (i * 15),
                55 + (i * 15),
                25,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF,
                1 + (HAL_RNG_GetRandomNumber(&hrng) % 16)
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Fill circle test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 10; i++) {
            ILI9341_FillCircle(&ili9341, 25 + (i * 20), 55 + (i * 20), 25, HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF);
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw ellipse test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_DrawEllipse(
                &ili9341,
                25 + (i * 15),
                55 + (i * 15),
                20,
                10 + (i * 5),
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw ellipse thick test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 15; i++) {
            ILI9341_DrawEllipseThick(
                &ili9341,
                25 + (i * 15),
                55 + (i * 15),
                20,
                10 + (i * 5),
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF,
                1 + (HAL_RNG_GetRandomNumber(&hrng) % 16)
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Fill ellipse test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = -5; i < 10; i++) {
            ILI9341_FillEllipse(
                &ili9341,
                25 + (i * 20),
                55 + (i * 20),
                20,
                10 + (i * 5),
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw polygon test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = 0; i < 10; i++) {
            uint16_t sides = 3 + (HAL_RNG_GetRandomNumber(&hrng) % 5);
            int16_t xs[sides];
            int16_t ys[sides];
            for (int16_t j = 0; j < sides; j++) {
                xs[j] = HAL_RNG_GetRandomNumber(&hrng) % ili9341.width;
                ys[j] = HAL_RNG_GetRandomNumber(&hrng) % ili9341.height;
            }
            ILI9341_DrawPolygon(&ili9341, xs, ys, sides, HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF);
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Draw polygon thick test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = 0; i < 10; i++) {
            uint16_t sides = 3 + (HAL_RNG_GetRandomNumber(&hrng) % 5);
            int16_t xs[sides];
            int16_t ys[sides];
            for (int16_t j = 0; j < sides; j++) {
                xs[j] = HAL_RNG_GetRandomNumber(&hrng) % ili9341.width;
                ys[j] = HAL_RNG_GetRandomNumber(&hrng) % ili9341.height;
            }
            ILI9341_DrawPolygonThick(
                &ili9341,
                xs,
                ys,
                sides,
                HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF,
                1 + (HAL_RNG_GetRandomNumber(&hrng) % 16),
                HAL_RNG_GetRandomNumber(&hrng) & 1
            );
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Fill polygon test",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        waitForButtonPress();
        for (int16_t i = 0; i < 10; i++) {
            uint16_t sides = 3 + (HAL_RNG_GetRandomNumber(&hrng) % 5);
            int16_t xs[sides];
            int16_t ys[sides];
            for (int16_t j = 0; j < sides; j++) {
                xs[j] = HAL_RNG_GetRandomNumber(&hrng) % ili9341.width;
                ys[j] = HAL_RNG_GetRandomNumber(&hrng) % ili9341.height;
            }
            ILI9341_FillPolygon(&ili9341, xs, ys, sides, HAL_RNG_GetRandomNumber(&hrng) % 0xFFFF);
        }
        HAL_Delay(250);
        waitForButtonPress();

        ILI9341_SetOrientation(&ili9341, ILI9341_ROTATION_VERTICAL_1);
        ILI9341_Touch_SetOrientation(&ili9341_touch, ILI9341_ROTATION_VERTICAL_1);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Touch test (vertical-1)",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        while (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_RESET) {
            if (ILI9341_Touch_IsPressed(&ili9341_touch)) {
                uint16_t x, y;
                if (ILI9341_Touch_GetCoordinates(&ili9341_touch, &x, &y)) {
                    ILI9341_FillCircle(&ili9341, x, y, 3, ILI9341_COLOR_RED);
                }
            }
        }

        ILI9341_SetOrientation(&ili9341, ILI9341_ROTATION_VERTICAL_2);
        ILI9341_Touch_SetOrientation(&ili9341_touch, ILI9341_ROTATION_VERTICAL_2);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Touch test (vertical-2)",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        while (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_RESET) {
            if (ILI9341_Touch_IsPressed(&ili9341_touch)) {
                uint16_t x, y;
                if (ILI9341_Touch_GetCoordinates(&ili9341_touch, &x, &y)) {
                    ILI9341_FillCircle(&ili9341, x, y, 3, ILI9341_COLOR_RED);
                }
            }
        }

        ILI9341_SetOrientation(&ili9341, ILI9341_ROTATION_HORIZONTAL_1);
        ILI9341_Touch_SetOrientation(&ili9341_touch, ILI9341_ROTATION_HORIZONTAL_1);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Touch test (horizontal-1)",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        while (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_RESET) {
            if (ILI9341_Touch_IsPressed(&ili9341_touch)) {
                uint16_t x, y;
                if (ILI9341_Touch_GetCoordinates(&ili9341_touch, &x, &y)) {
                    ILI9341_FillCircle(&ili9341, x, y, 3, ILI9341_COLOR_RED);
                }
            }
        }
        ILI9341_SetOrientation(&ili9341, ILI9341_ROTATION_HORIZONTAL_2);
        ILI9341_Touch_SetOrientation(&ili9341_touch, ILI9341_ROTATION_HORIZONTAL_2);
        ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
        ILI9341_WriteString(
            &ili9341,
            5,
            15,
            "Touch test (horizontal-2)",
            ILI9341_Font_Terminus8x16,
            ILI9341_COLOR_BLACK,
            ILI9341_COLOR_WHITE,
            false,
            1,
            0,
            0
        );
        HAL_Delay(250);
        while (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_RESET) {
            if (ILI9341_Touch_IsPressed(&ili9341_touch)) {
                uint16_t x, y;
                if (ILI9341_Touch_GetCoordinates(&ili9341_touch, &x, &y)) {
                    ILI9341_FillCircle(&ili9341, x, y, 3, ILI9341_COLOR_RED);
                }
            }
        }
        ILI9341_SetOrientation(&ili9341, ILI9341_ROTATION_HORIZONTAL_1);
        ILI9341_Touch_SetOrientation(&ili9341_touch, ILI9341_ROTATION_HORIZONTAL_1);
    }
}
