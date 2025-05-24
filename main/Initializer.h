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
#include "esp_timer.h"
#include "cJSON.h"


//#include "ESPHal.h"

//#include "cJSONSetUp.c"

#include "can.h"
#include "lvgl.h"

#define TAB1 0
#define TAB1_WATER_TEMP_INDEX 0
#define TAB1_OIL_PRESSURE_INDEX 1
#define TAB1_OIL_TEMP_INDEX 2
#define TAB1_RPM_INDEX 3
#define TAB1_FUEL_PRESSURE_INDEX 4
#define TAB1_MPH_INDEX 5
#define TAB1_GEAR_INDEX 6
#define TAB1_VOLTAGE_INDEX 7

#define TAB2 1
#define TAB2_TANK_PRESSURE_INDEX 0 //not in config?
#define TAB2_REGULATOR_PRESSURE_INDEX 1 //not in config?
#define TAB2_LAMBDA_INDEX 2
#define TAB2_ABSOLUTE_MANIFOLD_PRESSURE_INDEX 3
#define TAB2_FRONT_BRAKE_PRESSURE_INDEX 4
#define TAB2_BRAKE_BIAS_INDEX 5
#define TAB2_GEAR_POSITION_SOURCE_INDEX 6
#define TAB2_OIL_TEMP_INDEX 7
#define TAB2_OIL_PRESSURE_INDEX 8
#define TAB2_RPM_INDEX 9


#define OIL_TEMP_CONVERSION 0.1
#define WATER_TEMP_CONVERSION 0.1
#define OIL_PRESSURE_CONVERSION 0.0145038
#define ABSOLUTE_MANIFOLD_PRESSURE_CONVERSION 0.0145038
#define TANK_PRESSURE_CONVERSION 1
#define REGULATOR_PRESSURE_CONVERSION 1
#define FRONT_BRAKE_PRESSURE_CONVERSION 0.145038
#define LAMBDA_CONVERSION 0.01
#define BRAKE_BIAS_CONVERSION 1
#define VOLTAGE_CONVERSION 0.1
#define MPH_CONVERSION 0.0621
#define GEAR_CONVERSION 1
#define RPM_CONVERSION 1
#define FUEL_PRESSURE_CONVERSION 1
#define GEAR_POSITION_SOURCE_CONVERSION 1

#define TAB1NUMFIELDS 8
#define TAB2NUMFIELDS 8
#define NUMTABS 2

struct{
    lv_obj_t* label;
    lv_obj_t* bar;
    lv_obj_t* counter;
    int* value;
} typedef field;

struct{
    lv_obj_t* tab;
    lv_obj_t* button;
    field* fields;
    lv_obj_t* circle;
} typedef tab;

void setUpFields(tab tabs[], int tabCounter, char* things[], float* max, float* min);
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