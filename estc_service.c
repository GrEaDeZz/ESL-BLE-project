#include "estc_service.h"
#include "app_error.h"
#include "nrf_log.h"
#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service);

static void attr_value_setup(ble_gatts_attr_t * p_attr, ble_uuid_t * p_uuid, ble_gatts_attr_md_t * p_md, uint8_t * p_value, uint16_t length)
{
    memset(p_attr, 0, sizeof(ble_gatts_attr_t));
    
    p_attr->p_uuid    = p_uuid;
    p_attr->p_attr_md = p_md;
    p_attr->max_len   = length;
    p_attr->init_len  = length;
    p_attr->p_value   = p_value;
}

ret_code_t estc_ble_service_init(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid_t service_uuid;

    service_uuid.uuid = ESTC_SERVICE_UUID;

    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};

    error_code = sd_ble_uuid_vs_add(&base_uuid, &service->uuid_type);
    APP_ERROR_CHECK(error_code);

    service_uuid.type = service->uuid_type;

    error_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, 
                                          &service_uuid, 
                                          &service->service_handle);
    APP_ERROR_CHECK(error_code);

    NRF_LOG_DEBUG("%s:%d | Service UUID: 0x%04x", __FUNCTION__, __LINE__, service_uuid.uuid);
    NRF_LOG_DEBUG("%s:%d | Service UUID type: 0x%02x", __FUNCTION__, __LINE__, service_uuid.type);
    NRF_LOG_DEBUG("%s:%d | Service handle: 0x%04x", __FUNCTION__, __LINE__, service->service_handle);

    return estc_ble_add_characteristics(service);
}

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid_t char_state_uuid = {ESTC_GATT_CHAR_STATE_UUID, service->uuid_type};
    ble_uuid_t char_color_uuid = {ESTC_GATT_CHAR_COLOR_UUID, service->uuid_type};

    // Cвойства для характеристик
    ble_gatts_char_md_t char_md = { 0 };
    char_md.char_props.read   = 1; 
    char_md.char_props.write  = 1; 
    char_md.char_props.notify = 1;

    // Безопасность и хранение
    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_USER;
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&attr_md.write_perm);

    // Начальные значения
    service->led_state = 1;
    service->led_color[0] = 255;  // Red
    service->led_color[1] = 0;    // Green
    service->led_color[2] = 0;    // Blue

    ble_gatts_attr_t attr_char_value; 

    // 1. Добавляем xарактеристику состояния
    attr_value_setup(&attr_char_value, &char_state_uuid, &attr_md, &service->led_state, sizeof(service->led_state));
    error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, &service->led_state_handle);
    APP_ERROR_CHECK(error_code);

    // 2. Добавляем xарактеристику цвета
    attr_value_setup(&attr_char_value, &char_color_uuid, &attr_md, service->led_color, sizeof(service->led_color));
    error_code = sd_ble_gatts_characteristic_add(service->service_handle, &char_md, &attr_char_value, &service->led_color_handle);
    APP_ERROR_CHECK(error_code);

    return NRF_SUCCESS;
}