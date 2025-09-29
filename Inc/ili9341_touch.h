#ifndef __ILI9341_TOUCH_H__
#define __ILI9341_TOUCH_H__

#include "ili9341.h"
#include "stdbool.h"
#include "stdint.h"
#include "stm32f7xx_hal.h"

// Raw touch values range, may require calibration
// to calibrate uncomment UART_Printf line in ili9341_touch.c
#define ILI9341_TOUCH_MIN_RAW_X 2000
#define ILI9341_TOUCH_MAX_RAW_X 30750
#define ILI9341_TOUCH_MIN_RAW_Y 2000
#define ILI9341_TOUCH_MAX_RAW_Y 31000

/**
 * @brief ILI9341 touch controller handle structure
 */
typedef struct {
    SPI_HandleTypeDef* spi_handle;
    GPIO_TypeDef* cs_port;
    uint16_t cs_pin;
    GPIO_TypeDef* irq_port;
    uint16_t irq_pin;
    int_fast8_t rotation;
    int_fast16_t width;
    int_fast16_t height;
} ILI9341_Touch_HandleTypeDef;

/**
 * @brief Deselect the ILI9341 touch controller, call before using other peripherals on the same SPI bus
 * @param ili9341_touch Pointer to the ILI9341_Touch_HandleTypeDef structure
 */
void ILI9341_Touch_Deselect(const ILI9341_Touch_HandleTypeDef* ili9341_touch);

/**
 * @brief Initialize the touch controller
 * @param spi_handle Pointer to the SPI_HandleTypeDef structure
 * @param cs_port GPIO port for the chip select pin
 * @param cs_pin GPIO pin number for the chip select pin
 * @param irq_port GPIO port for the IRQ pin
 * @param irq_pin GPIO pin number for the IRQ pin
 * @param rotation Initial display rotation, one of ILI9341_ROTATION_* values
 * @param width Display width in pixels
 * @param height Display height in pixels
 * @return Initialized ILI9341_Touch_HandleTypeDef structure
 */
ILI9341_Touch_HandleTypeDef ILI9341_Touch_Init(
    SPI_HandleTypeDef* spi_handle,
    GPIO_TypeDef* cs_port,
    uint16_t cs_pin,
    GPIO_TypeDef* irq_port,
    uint16_t irq_pin,
    int_fast8_t rotation,
    int_fast16_t width,
    int_fast16_t height
);

/**
 * @brief Set the display rotation
 * @param ili9341_touch Pointer to the ILI9341_Touch_HandleTypeDef structure
 * @param rotation New display rotation, one of ILI9341_ROTATION_* values
 */
void ILI9341_Touch_SetOrientation(ILI9341_Touch_HandleTypeDef* ili9341_touch, int_fast8_t rotation);

/**
 * @brief Check if the touchscreen is currently pressed
 * @param ili9341_touch Pointer to the ILI9341_Touch_HandleTypeDef structure
 */
bool ILI9341_Touch_IsPressed(const ILI9341_Touch_HandleTypeDef* ili9341_touch);

/**
 * @brief Get the current touch coordinates
 * @param ili9341_touch Pointer to the ILI9341_Touch_HandleTypeDef structure
 * @param x Pointer to store the X coordinate
 * @param y Pointer to store the Y coordinate
 * @return true if coordinates were successfully read, false otherwise
 */
bool ILI9341_Touch_GetCoordinates(const ILI9341_Touch_HandleTypeDef* ili9341_touch, uint16_t* x, uint16_t* y);

#endif  // __ILI9341_TOUCH_H__
