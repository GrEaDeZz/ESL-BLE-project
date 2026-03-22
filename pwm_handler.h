#ifndef PWM_HANDLER_H
#define PWM_HANDLER_H

#include <stdint.h>

// Инициализация модуля ШИМ
void pwm_handler_init(const uint32_t *led_pins);

// Установка цвета RGB (значения 0-1000)
void pwm_handler_set_rgb(uint16_t r, uint16_t g, uint16_t b);

#endif