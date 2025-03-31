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
#define WATER_TEMP_INDEX 0
#define OIL_PRESSURE_INDEX 1
#define OIL_TEMP_INDEX 2
#define RPM_INDEX 3
#define FUEL_PRESSURE_INDEX 4
#define MPH_INDEX 5
#define GEAR_INDEX 6
#define VOLTAGE_INDEX 7

#define TAB2 1
#define TANK_PRESSURE_INDEX 0 //not in config?
#define REGULATOR_PRESSURE_INDEX 1 //not in config?
#define LAMBDA_INDEX 2
#define ABSOLUTE_MANIFOLD_PRESSURE_INDEX 3
#define FRONT_BRAKE_PRESSURE_INDEX 4
#define BRAKE_BIAS_INDEX 5
#define GEAR_POSITION_SOURCE_INDEX 6
#define TAB2_OIL_TEMPERATURE_INDEX 7
#define TAB2_OIL_PRESSURE_INDEX 8
#define TAB2_RPM_INDEX 9


#define OIL_TEMP_CONVERSION 0.1
#define WATER_TEMP_CONVERSION 0.1
#define OIL_PRESSURE_CONVERSION 0.0145038
#define ABSOLUTE_MANIFOLD_PRESSURE_CONVERSION 0.0145038
#define TANK_PRESSURE_CONVERSION 0.145038
#define REGULATOR_PRESSURE_CONVERSION 0.0145038
#define FRONT_BRAKE_PRESSURE_CONVERSION 0.145038
#define LAMBDA_CONVERSION 0.01

#define TAB1NUMFIELDS 8
#define TAB2NUMFIELDS 8
#define NUMTABS 2

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

void setUpFields(tab tabs[], int tabCounter, char* things[], float* max, float* min);
lv_obj_t * create_progress_bar(lv_obj_t *parent, int x, int y, float max, float min);
void updateObject(field object, int value, int index, int page);
lv_obj_t * create_label(lv_obj_t *parent, int x, int y, char* name, int tabNum);
lv_obj_t * create_counter(lv_obj_t *parent, int x, int y, int tabNum);
void changePage(field* fields);
void btn_event_cb(lv_event_t *e);
void setTabView(lv_obj_t * e);
void makeButton(tab tabs[], int);
void makeCircle(tab*, int);
void warning(tab tabs[]);
void updateArray(tab tabs[]);
void button_init();
void button_task(void *arg);
// esp_err_t SD_init(sdmmc_slot_config_t* SD_config);