#include "can.h"


static const char *CAN = "CAN";

twai_message_t recieve_CAN(){
    twai_message_t ret;
    ret.identifier = 0x00;
    if (twai_receive(&ret, pdMS_TO_TICKS(50)) == ESP_OK) {
        // ESP_LOGI(CAN,"hello recieved something %x,%x",ret.data[0],ret.data[1]);
    return ret;
    } 
    else {
    return ret;
    }
}

void configure_CAN(){
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_20,GPIO_NUM_19,TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_1MBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

     if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
        ESP_LOGI(CAN, "Driver initialized\n ");
    } else {
        ESP_LOGE(CAN, "Driver failed to initialize\n ");
        return;
    }
    if (twai_start() == ESP_OK) {
        ESP_LOGI(CAN, "Driver started\n ");

    } else {
        ESP_LOGE(CAN, "Failed to start driver\n ");

        return;
    }
}