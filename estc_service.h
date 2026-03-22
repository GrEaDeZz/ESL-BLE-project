#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESTC_BASE_UUID { 0x38, 0x1F, 0xBE, 0xC2, 0xA8, 0xA8, 0x27, 0x8F, 0xC1, 0x48, 0xD6, 0x38, 0x00, 0x00, 0x6D, 0xD7 } // UUID: d76dec72-38d6-48c1-8f27-a8a8c2be1f38

#define ESTC_SERVICE_UUID 0xEC72

#define ESTC_GATT_CHAR_STATE_UUID 0x6066

#define ESTC_GATT_CHAR_COLOR_UUID 0x6067

typedef struct
{
    uint8_t  uuid_type;
    uint16_t service_handle;
    uint16_t connection_handle;

    ble_gatts_char_handles_t led_state_handle;
    ble_gatts_char_handles_t led_color_handle;

    uint8_t  led_state;
    uint8_t  led_color[3];
} ble_estc_service_t;

ret_code_t estc_ble_service_init(ble_estc_service_t *service);

#endif /* ESTC_SERVICE_H__ */