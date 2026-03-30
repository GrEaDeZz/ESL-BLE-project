#ifndef PWM_HANDLER_H
#define PWM_HANDLER_H

#include <stdint.h>

// Инициализация модуля ШИМ
void pwm_handler_init(const uint32_t *led_pins);

// Установка цвета RGB (значения 0-255)
void pwm_handler_set_rgb(uint8_t r, uint8_t g, uint8_t b);

#endif