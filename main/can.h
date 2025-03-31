#include "driver/twai.h"
#include "waveshare_rgb_lcd_port.h"
#include "driver/gpio.h"
#include "esp_log.h"

void configure_CAN();
int send_CAN(uint32_t ID,uint8_t len,uint8_t * data);
twai_message_t recieve_CAN();
void setupCANLED(uint32_t pin);
void blinkCANLED(uint32_t pin);