/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "waveshare_rgb_lcd_port.h"
#include "Initializer.h"
#include "lvgl.h"
#include "can.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_intr_alloc.h"
#include "esp_timer.h"
#include<unistd.h>

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"

#include "MizzouRacingLogoTransparent.c"



#define GPIO_INPUT_IO  6  // Change to your GPIO pin
#define ESP_INTR_FLAG_DEFAULT 0

// int counter;
lv_obj_t *tabview;

volatile int64_t last_interrupt_time = 0;
int64_t lastUpdateTime;
int64_t currentUpdateTime;
volatile int counter = 0;
volatile int changed = 0;
volatile int disabled = 0;

void IRAM_ATTR gpio_isr_handler(void* arg) {//debounce
    gpio_intr_disable(GPIO_INPUT_IO);
    counter = !counter;
    disabled = 1;
}

void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init(); // Initialize the Waveshare ESP32-S3 RGB LCD 
    // wavesahre_rgb_lcd_bl_on();  //Turn on the screen backlight 
    // wavesahre_rgb_lcd_bl_off(); //Turn off the screen backlight 
    
    ESP_LOGI(TAG, "Display LVGL demos");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lvgl_port_lock(-1)) {
        
        // // lv_demo_stress();
        // // lv_demo_benchmark();
        // // lv_demo_music();
        // lv_demo_widgets();
        // // example_lvgl_demo_ui();
        // // Release the mutex
        
        lv_obj_t *scr = lv_scr_act();
        LV_IMG_DECLARE(MizzouRacingLogoBlackBackground);
        lv_obj_t *img = lv_img_create(scr);
        lv_obj_set_style_bg_color(scr, lv_palette_lighten(LV_PALETTE_NONE, 1), LV_PART_MAIN);
        lv_img_set_src(img, &MizzouRacingLogoBlackBackground);
        lv_obj_center(img);
        lvgl_port_unlock();
        vTaskDelay(2000);

        

        // sdmmc_slot_config_t SD = SDMMC_SLOT_CONFIG_DEFAULT();
        // SD.width = 128; //maybe
        // SD.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;//def not right
        // SD.d0 = 37; //def not right
        // SD.clk = 36;//def not right
        // SD.cmd = 35;//def not right
        // ESP_ERROR_CHECK(SD_init(&SD));
        lvgl_port_lock(-1);
        char* tab1Labels[50] = {"Water Temp (F)", "Oil Pressure (psi)", "Oil Temp (F)", "RPM", "Fuel Pressure (psi)", "MPH", "Gear", "Voltage (V)", "Tank Pressure (psi)", "Reg Pressure (psi)", "Lambda (LA)", "Manif Pressure (psi)", "F Brake Pressure (psi)", "Brake Bias (%)", "Gear Position Source", "Oil Temp (F)", "Oil Pressure (psi)"};
        float maxes[] = {250, 100, 250, 15000, 60, 90, 6, 145, 2000, 250, 255, 20, 2000, 100, 2, 250, 100};
        float mins[] = {32, 0, 32, 0, 0, 0, 0, 90, 0, 0, 0, 0, 0, 0, 0, 32, 0};//get rid of these
        tabview = lv_tabview_create(scr, LV_DIR_TOP, 0);
        setTabView(tabview);
        tab tabs[2];
        for(int i = 0; i<2; i++){
            tabs[i].tab = lv_tabview_add_tab(tabview, "Tab 1");
            //lv_obj_set_size(tabs[i].tab, 800, 480);
            lv_obj_clear_flag(tabs[i].tab, LV_OBJ_FLAG_SCROLLABLE);
            if(!i){
                makeCircle(tabs, i);
            }
            setUpFields(tabs, i, tab1Labels, maxes, mins);
        }

        //setTabID(TAB1);
        lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_YELLOW, 1), LV_PART_MAIN);

        // float testHigh[8] = {200, 90, 225, 14000, 55, 80, 6, 14};
        // float testLow[8] = {40, 10, 40, 1000, 20, 20, 0, 9.5};
        // lv_obj_set_size(tabview, 800, 480);
        // lv_obj_clear_flag(tabview, LV_OBJ_FLAG_SCROLLABLE);

        configure_CAN();

        
        //button_init();

        xTaskCreatePinnedToCore(warning, "WARNING", 4096, tabs, 5, NULL, 1);

        //xTaskCreate(button_task, "BUTTON", 4096, NULL, 6, NULL);

        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_NEGEDGE;  // Interrupt on falling edge
        io_conf.pin_bit_mask = (1ULL << GPIO_INPUT_IO);
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pull_up_en = 1;  // Enable pull-up resistor
        gpio_config(&io_conf);
        gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
        gpio_isr_handler_add(GPIO_INPUT_IO, gpio_isr_handler, (void*) &counter);

        
        
        lvgl_port_unlock();
        while(1){
            if(counter != changed){
                // float filter = 0;
                // currentUpdateTime = esp_timer_get_time();
                // while((esp_timer_get_time() - currentUpdateTime)/1000 > 3){
                //     filter = filter * 0.1 + 0.9 * (1-gpio_get_level(GPIO_INPUT_IO));
                // }
                // if(filter > 0.75){
                usleep(5);
                if(!gpio_get_level(GPIO_INPUT_IO)){
                    lvgl_port_lock(-1);
                    //lv_tabview_set_act(tabview, counter, LV_ANIM_OFF);
                    switchTabID();
                    switchTabView();
                    changed = counter;
                    lv_obj_invalidate(tabs[counter].tab);
                    
                    lvgl_port_unlock();
                    last_interrupt_time = esp_timer_get_time();
                    //printf("succeeded to change\n");
                    usleep(0);
                }
                else{
                    counter = !counter;
                    disabled = 0;
                    //printf("%d    failed to change\n", disabled);
                    gpio_intr_enable(GPIO_INPUT_IO);
                    usleep(0);
                }
            }
            currentUpdateTime = esp_timer_get_time();
            if((currentUpdateTime - lastUpdateTime)/1000 > 100){
                lastUpdateTime = currentUpdateTime;
                //lvgl_port_lock(-1);
                for(int i = 0; i<6; i++){
                    updateArray(tabs);
                }
                //lvgl_port_unlock();
                usleep(0);
            }
            if(disabled && ((currentUpdateTime - last_interrupt_time)/1000 > 400)){
                //printf("asdf\n");
                disabled = 0;
                gpio_intr_enable(GPIO_INPUT_IO);
                usleep(0);
            }
            //printf("%d\n", gpio_get_level(GPIO_INPUT_IO));
        }
    }
}
