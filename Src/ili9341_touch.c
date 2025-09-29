#include "ili9341_touch.h"

#include "stm32f7xx_hal.h"

/**
 * @brief Select the ILI9341 touch controller
 * @param ili9341_touch Pointer to the ILI9341_Touch_HandleTypeDef structure
 */
static void ILI9341_Touch_Select(const ILI9341_Touch_HandleTypeDef* ili9341_touch) {
    HAL_GPIO_WritePin(ili9341_touch->cs_port, ili9341_touch->cs_pin, GPIO_PIN_RESET);
}

void ILI9341_Touch_Deselect(const ILI9341_Touch_HandleTypeDef* ili9341_touch) {
    HAL_GPIO_WritePin(ili9341_touch->cs_port, ili9341_touch->cs_pin, GPIO_PIN_SET);
}

ILI9341_Touch_HandleTypeDef ILI9341_Touch_Init(
    SPI_HandleTypeDef* spi_handle,
    GPIO_TypeDef* cs_port,
    uint16_t cs_pin,
    GPIO_TypeDef* irq_port,
    uint16_t irq_pin,
    int_fast8_t rotation,
    int_fast16_t width,
    int_fast16_t height
) {
    width = abs(width);
    height = abs(height);

    const ILI9341_Touch_HandleTypeDef ili9341_touch_instance = {
        .spi_handle = spi_handle,
        .cs_port = cs_port,
        .cs_pin = cs_pin,
        .irq_port = irq_port,
        .irq_pin = irq_pin,
        .rotation = rotation,
        .width = width,
        .height = height
    };

    ILI9341_Touch_Deselect(&ili9341_touch_instance);

    return ili9341_touch_instance;
}

void ILI9341_Touch_SetOrientation(ILI9341_Touch_HandleTypeDef* ili9341_touch, int_fast8_t rotation) {
    if ((ili9341_touch->rotation == ILI9341_ROTATION_HORIZONTAL_1 ||
         ili9341_touch->rotation == ILI9341_ROTATION_HORIZONTAL_2) &&
        (rotation == ILI9341_ROTATION_VERTICAL_1 || rotation == ILI9341_ROTATION_VERTICAL_2)) {
        uint16_t temp = ili9341_touch->width;
        ili9341_touch->width = ili9341_touch->height;
        ili9341_touch->height = temp;
    } else if ((ili9341_touch->rotation == ILI9341_ROTATION_VERTICAL_1 ||
                ili9341_touch->rotation == ILI9341_ROTATION_VERTICAL_2) &&
               (rotation == ILI9341_ROTATION_HORIZONTAL_1 || rotation == ILI9341_ROTATION_HORIZONTAL_2)) {
        uint16_t temp = ili9341_touch->width;
        ili9341_touch->width = ili9341_touch->height;
        ili9341_touch->height = temp;
    }

    ili9341_touch->rotation = rotation;
}

bool ILI9341_Touch_IsPressed(const ILI9341_Touch_HandleTypeDef* ili9341_touch) {
    return HAL_GPIO_ReadPin(ili9341_touch->irq_port, ili9341_touch->irq_pin) == GPIO_PIN_RESET;
}

bool ILI9341_Touch_GetCoordinates(const ILI9341_Touch_HandleTypeDef* ili9341_touch, uint16_t* x, uint16_t* y) {
    static const uint8_t cmdReadX[] = {0xD0};
    static const uint8_t cmdReadY[] = {0x90};
    static const uint8_t zeroes[] = {0x00, 0x00};

    ILI9341_Touch_Select(ili9341_touch);

    int_fast32_t avgX = 0;
    int_fast32_t avgY = 0;
    int_fast8_t nsamples = 0;
    for (int_fast8_t i = 0; i < 16; i++) {
        if (!ILI9341_Touch_IsPressed(ili9341_touch)) break;

        nsamples++;

        HAL_SPI_Transmit(ili9341_touch->spi_handle, (uint8_t*)cmdReadY, sizeof(cmdReadY), HAL_MAX_DELAY);
        uint8_t yRaw[2];
        HAL_SPI_TransmitReceive(ili9341_touch->spi_handle, (uint8_t*)zeroes, yRaw, sizeof(yRaw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(ili9341_touch->spi_handle, (uint8_t*)cmdReadX, sizeof(cmdReadX), HAL_MAX_DELAY);
        uint8_t xRaw[2];
        HAL_SPI_TransmitReceive(ili9341_touch->spi_handle, (uint8_t*)zeroes, xRaw, sizeof(xRaw), HAL_MAX_DELAY);

        avgX += (((int_fast16_t)xRaw[0]) << 8) | ((int_fast16_t)xRaw[1]);
        avgY += (((int_fast16_t)yRaw[0]) << 8) | ((int_fast16_t)yRaw[1]);
    }

    ILI9341_Touch_Deselect(ili9341_touch);

    if (nsamples < 16) return false;

    int_fast32_t rawX = (avgX / 16);
    int_fast32_t rawY = (avgY / 16);

    // Uncomment this line and implement/change UART_Printf to calibrate touchscreen:
    // UART_Printf("rawX = %d, rawY = %d\r\n", rawX, rawY);

    if (rawX < ILI9341_TOUCH_MIN_RAW_X) rawX = ILI9341_TOUCH_MIN_RAW_X;
    if (rawX > ILI9341_TOUCH_MAX_RAW_X) rawX = ILI9341_TOUCH_MAX_RAW_X;

    if (rawY < ILI9341_TOUCH_MIN_RAW_Y) rawY = ILI9341_TOUCH_MIN_RAW_Y;
    if (rawY > ILI9341_TOUCH_MAX_RAW_Y) rawY = ILI9341_TOUCH_MAX_RAW_Y;

    // x = (rawX - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->width /
    // (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
    // y = (rawY - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->height /
    // (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);

    switch (ili9341_touch->rotation) {
        case ILI9341_ROTATION_HORIZONTAL_1:
            *y = (rawX - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->height /
                 (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
            *x = (rawY - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->width /
                 (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);
            break;
        case ILI9341_ROTATION_VERTICAL_1:
            *x = (rawX - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->width /
                 (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
            *y = ili9341_touch->height - 1 -
                 (rawY - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->height /
                     (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);
            break;
        case ILI9341_ROTATION_HORIZONTAL_2:
            *y = ili9341_touch->height - 1 -
                 (rawX - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->height /
                     (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
            *x = ili9341_touch->width - 1 -
                 (rawY - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->width /
                     (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);
            break;
        case ILI9341_ROTATION_VERTICAL_2:
            *x = ili9341_touch->width - 1 -
                 (rawX - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->width /
                     (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
            *y = (rawY - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->height /
                 (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);
            break;
        default:
            *x = (rawX - ILI9341_TOUCH_MIN_RAW_X) * ili9341_touch->width /
                 (ILI9341_TOUCH_MAX_RAW_X - ILI9341_TOUCH_MIN_RAW_X);
            *y = (rawY - ILI9341_TOUCH_MIN_RAW_Y) * ili9341_touch->height /
                 (ILI9341_TOUCH_MAX_RAW_Y - ILI9341_TOUCH_MIN_RAW_Y);
            break;
    }

    return true;
}
