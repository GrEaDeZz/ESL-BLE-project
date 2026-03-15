#ifndef ESTC_SERVICE_H__
#define ESTC_SERVICE_H__

#include <stdint.h>

#include "ble.h"
#include "sdk_errors.h"

#define ESTC_BASE_UUID { 0x38, 0x1F, 0xBE, 0xC2, 0xA8, 0xA8, 0x27, 0x8F, 0xC1, 0x48, 0xD6, 0x38, 0x00, 0x00, 0x6D, 0xD7 } // UUID: d76dec72-38d6-48c1-8f27-a8a8c2be1f38

#define ESTC_SERVICE_UUID 0xEC72

#define ESTC_GATT_CHAR_1_UUID 0x6066

#define ESTC_GATT_CHAR_2_UUID 0x6067

#define ESTC_GATT_CHAR_3_UUID 0x6068

typedef struct
{
    uint16_t service_handle;
    uint16_t connection_handle;
    ble_gatts_char_handles_t char_1_handle;
    ble_gatts_char_handles_t char_2_handle;
    ble_gatts_char_handles_t char_3_handle;
} ble_estc_service_t;

ret_code_t estc_ble_service_init(ble_estc_service_t *service);

#endif /* ESTC_SERVICE_H__ */