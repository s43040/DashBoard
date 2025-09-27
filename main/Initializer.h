#include "waveshare_rgb_lcd_port.h"
#include <stdlib.h>
#include <stdio.h>
#include "driver/twai.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


#include <stdlib.h>
#include "can.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_sntp.h"
#include "hal/gpio_types.h"
#include "driver/gpio.h"

#include "cJSON.h"


//#include "ESPHal.h"

//#include "cJSONSetUp.c"

#include "can.h"
#include "lvgl.h"

#define TAB1 0
#define TAB1_PACK_VOLTAGE_INDEX 0 //DONE
#define TAB1_LOWEST_CELL_VOLTAGE_INDEX 1 //DONE
#define TAB1_GL_VOLTAGE_INDEX 2 //DONE
#define TAB1_RPM_INDEX 3 //DONE
#define TAB1_PACK_TEMP_INDEX 4 //DONE
#define TAB1_INVERTER_TEMP_INDEX 5 //DONE
#define TAB1_MOTOR_TEMP_INDEX 6 //DONE
#define TAB1_HIGHEST_CELL_TEMP_INDEX 7 //DONE


#define TAB2 1
#define TAB2_REAR_BREAK_PRESSURE_INDEX 0 //DONE
#define TAB2_BRAKE_BIAS_INDEX 1 //DONE
#define TAB2_CELL_BALANCE_DELTA_INDEX 2

#define TAB2_RPM_INDEX 9



#define PACK_VOLTAGE_CONVERSION 0.1
#define MOTOR_TEMP_CONVERSION 0.1
#define GL_VOLTAGE_CONVERSION 0.1
#define RPM_CONVERSION 0.1
#define PACK_TEMP_CONVERSION 0.1
#define HIGHEST_CELL_TEMP_CONVERSION 1
#define INVERTER_TEMP_CONVERSION 0.1
#define REAR_BREAK_PRESSURE_CONVERSION 0.1
#define BRAKE_BIAS_CONVERSION 0.01
#define LOWEST_CELL_VOLTAGE_CONVERSION 0.0001
#define CELL_BALANCE_DELTA_CONVERSION 0.1

#define TAB1NUMFIELDS 8
#define NUMTABS 2
#define NUM_CASES 5

struct{
    lv_obj_t* label;
    lv_obj_t* bar;
    lv_obj_t* counter;
    float* value;
} typedef field;

struct{
    lv_obj_t* tab;
    lv_obj_t* button;
    field* fields;
    lv_obj_t* circle;
} typedef tab;

void setUpFields(tab tabs[], int tabCounter, char* names[]);
lv_obj_t * create_progress_bar(lv_obj_t *parent, int x, int y, float max, float min);
void updateObject(field object, float value, int index, int page);
lv_obj_t * create_label(lv_obj_t *parent, int x, int y, char* name, int tabNum);
lv_obj_t * create_counter(lv_obj_t *parent, int x, int y, int tabNum);
void changePage(field* fields);
void btn_event_cb(lv_event_t *e);
void setTabView(lv_obj_t * e);
void makeButton(tab tabs[], int);
void makeCircle(tab*, int);
void warning(tab tabs[]);
void updateArray(tab tabs[], twai_message_t message);
void button_init();
void button_task(void *arg);
void switchTabID();
int getTabID();
void switchTabView();
// esp_err_t SD_init(sdmmc_slot_config_t* SD_config);