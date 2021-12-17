#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "led.h"
#include "ui_task.h"
#include "ui.h"

#include "esp_adc_cal.h"


//ADC Channels
#define ADC1_EXAMPLE_CHAN0          ADC1_CHANNEL_5
static const char *TAG_CH[2][10] = {{"ADC1_CH5"}, {"ADC2_CH0"}};

//ADC Attenuation
#define ADC_EXAMPLE_ATTEN           ADC_ATTEN_DB_11

#define ADC_EXAMPLE_CALI_SCHEME     ESP_ADC_CAL_VAL_EFUSE_TP_FIT

static int adc_raw[2][10];
static const char *TAG = "ADC SINGLE";

static esp_adc_cal_characteristics_t adc1_chars;

static bool adc_calibration_init(void)
{
    esp_err_t ret;
    bool cali_enable = false;

    ret = esp_adc_cal_check_efuse(ADC_EXAMPLE_CALI_SCHEME);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        cali_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_EXAMPLE_ATTEN, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    } else {
        ESP_LOGE(TAG, "Invalid arg");
    }

    return cali_enable;
}

bool CALI_ENABLE;

void adcinit(void)
{
    esp_err_t ret = ESP_OK;
    // uint32_t voltage = 0;
    CALI_ENABLE = adc_calibration_init();

    // ADC1 config
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_EXAMPLE_CHAN0, ADC_EXAMPLE_ATTEN));
}

void readadc(void)
{
    uint32_t voltage = 0;
    adc_raw[0][0] = adc1_get_raw(ADC1_EXAMPLE_CHAN0);
    ESP_LOGI(TAG_CH[0][0], "raw  data: %d", adc_raw[0][0]);
    if (CALI_ENABLE)
    {
        voltage = esp_adc_cal_raw_to_voltage(adc_raw[0][0], &adc1_chars);
        ESP_LOGI(TAG_CH[0][0], "cali data: %d mV", voltage);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
}

void chip_info_print(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}

void app_main(void)
{
    chip_info_print();
    eye_init();
    ui_task_init();
    vTaskDelay(pdMS_TO_TICKS(1500));
    lv_example_style_1();
    adcinit();
    while (1)
    {
        readadc();
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
