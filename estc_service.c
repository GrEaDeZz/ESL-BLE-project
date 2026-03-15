#include "estc_service.h"

#include "app_error.h"
#include "nrf_log.h"

#include "ble.h"
#include "ble_gatts.h"
#include "ble_srv_common.h"

static ret_code_t estc_ble_add_characteristics(ble_estc_service_t *service);

ret_code_t estc_ble_service_init(ble_estc_service_t *service)
{
    ret_code_t error_code = NRF_SUCCESS;

    ble_uuid_t service_uuid;

    service_uuid.uuid = ESTC_SERVICE_UUID;

    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};

    error_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(error_code);

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

    // 6.1. Регистрируем UUID трех характеристик
    ble_uuid_t char_uuid1, char_uuid2, char_uuid3;
    char_uuid1.uuid = ESTC_GATT_CHAR_1_UUID;
    char_uuid2.uuid = ESTC_GATT_CHAR_2_UUID;
    char_uuid3.uuid = ESTC_GATT_CHAR_3_UUID;
    ble_uuid128_t base_uuid = {ESTC_BASE_UUID};
    error_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid1.type);
    APP_ERROR_CHECK(error_code);
    error_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid2.type);
    APP_ERROR_CHECK(error_code);
    error_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid3.type);
    APP_ERROR_CHECK(error_code);

    // 6.1.1 Добавляем описание к нашей первой характеристики
    ble_gatts_attr_md_t desc_md = { 0 };
    desc_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&desc_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&desc_md.write_perm);
    
    uint8_t user_desc[] = "Characteristic description";

    // 6.5. Настраиваем свойства + Прикрепляем дескриптор описания характеристики 1
    ble_gatts_char_md_t char_md1 = { 0 }, char_md2 = { 0 }, char_md3 = { 0 };
    char_md1.char_props.read     = 1;
    char_md1.char_props.write    = 1;
    char_md2.char_props.read     = 1;
    char_md2.char_props.write    = 1;
    char_md2.char_props.indicate = 1;
    char_md3.char_props.read     = 1;
    char_md3.char_props.write    = 1;
    char_md3.char_props.notify   = 1;

    char_md1.p_char_user_desc        = user_desc;
    char_md1.char_user_desc_size     = sizeof(user_desc);
    char_md1.char_user_desc_max_size = sizeof(user_desc);
    char_md1.p_user_desc_md          = &desc_md;

    // 6.6. Настраиваем безопасность
    ble_gatts_attr_md_t attr_md = { 0 };
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    // 6.2 и 6.7. Собираем в итоговые атрибуты
    ble_gatts_attr_t attr_char_value1 = { 0 }, attr_char_value2 = { 0 }, attr_char_value3 = { 0 };
    attr_char_value1.p_uuid    = &char_uuid1;
    attr_char_value1.p_attr_md = &attr_md;
    attr_char_value1.max_len   = sizeof(int32_t);
    attr_char_value1.init_len  = sizeof(int32_t);

    attr_char_value2.p_uuid    = &char_uuid2;
    attr_char_value2.p_attr_md = &attr_md;
    attr_char_value2.max_len   = sizeof(int32_t);
    attr_char_value2.init_len  = sizeof(int32_t);

    attr_char_value3.p_uuid    = &char_uuid3;
    attr_char_value3.p_attr_md = &attr_md;
    attr_char_value3.max_len   = sizeof(int32_t);
    attr_char_value3.init_len  = sizeof(int32_t);
    
    uint8_t init_value[4] = {0x00, 0x00, 0x00, 0x00};
    attr_char_value1.p_value   = init_value;
    attr_char_value2.p_value   = init_value;
    attr_char_value3.p_value   = init_value;

    // 6.4. Привязываем характеристики к нашему сервису
    error_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                                 &char_md1,
                                                 &attr_char_value1,
                                                 &service->char_1_handle);
    APP_ERROR_CHECK(error_code);
    error_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                                 &char_md2,
                                                 &attr_char_value2,
                                                 &service->char_2_handle);
    APP_ERROR_CHECK(error_code);
    error_code = sd_ble_gatts_characteristic_add(service->service_handle,
                                                 &char_md3,
                                                 &attr_char_value3,
                                                 &service->char_3_handle);
    APP_ERROR_CHECK(error_code);

    return NRF_SUCCESS;
}
