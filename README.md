# STM32-ILI9341

STM32 HAL-based library for ILI9341 TFT modules with touchscreen.

This library is a fork of [afiskon/stm32-ili9341][u0] which is based on [afiskon/stm32-st7735][u1] and
[afiskon/stm32-ssd1306][u2]. Initialization sequence for ILI9341 was borrowed
from [martnak/STM32-ILI9341][u3].

See also:

- https://github.com/afiskon/stm32-i2c-lcd-1602
- https://github.com/afiskon/stm32-ssd1306
- https://github.com/afiskon/stm32-ssd1351
- https://github.com/afiskon/stm32-st7735

[u0]: https://github.com/afiskon/stm32-ili9341
[u1]: https://github.com/afiskon/stm32-st7735
[u2]: https://github.com/afiskon/stm32-ssd1306
[u3]: https://github.com/martnak/STM32-ILI9341

## Usage

1. Initialize the ILI9341 and/or the touch controller using the init functions (the display and the touchscreen can be used independently)

```c
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
```

2. Use functions to do stuffs, always pass the pointer to the handle as the first argument so the function know what display you want to manipulate. You can have multiple displays and even on the same SPI bus, you'll need to initialize them separately and manipulate them one at a time.

```c
ILI9341_FillScreen(&ili9341_1, ILI9341_COLOR_WHITE);
ILI9341_FillScreen(&ili9341_2, ILI9341_COLOR_WHITE);
```

More informations and documentations are available in the header files. Examples and functionality tests are available in the [example](./example.c)

## Notes

1. To use Thai font, you'll need to encode strings in the executable using code page ISO-8859-11, to do this you'll need to add the compiler options `-finput-charset=UTF-8 -fexec-charset=ISO-8859-11` which can be accomplished by adding the following lines to CMakeLists.txt

```
# Allow Thai text to be encoded in a single byte
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -finput-charset=UTF-8
    -fexec-charset=ISO-8859-11
)
```

2. To add custom font, use the [export_font.py](./export_font.py) script (place it in a folder along with .bdf files and run it), this is not a "production-ready" script and may require modifications to use with some fonts. After you generate the font data file with the script, rename it appropiately and add it to the Src folder, then add the font declarations to the [header file](./Inc/ili9341_fonts.h). Note that only .bdf bitmap fonts are supported currently.

### Original license for [afiskon/stm32-ili9341][u0] (upstream of this fork)

```
MIT License

Copyright (c) 2018 Aleksander Alekseev

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
```
