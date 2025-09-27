#include "Initializer.h"


#define MOUNT_POINT "/sdcard"

static lv_obj_t * tabview;
int tab_id = 0;
int errorCode = 0;
int WaterTempError = 0;
int VoltageError = 0;
int OilPressureError = 0;
int OilTempError = 0;
int goodZone = 1;
static lv_style_t style_indic;

int screenChange = 0;

void setUpFields(tab tabs[], int tabCounter, char* names[]){
    tabs[tabCounter].fields = (field*)malloc(10*sizeof(field));
    for(int i = 0; i<10; i++){
        tabs[tabCounter].fields[i].value = (float*)malloc(sizeof(float));
    }

    int xcoordinate, ycoordinate;
    if(!tabCounter){
        for(int i = 0; i<3; i++){
            xcoordinate = -270+270*i; ycoordinate = -65+abs(-30+30*i);
            tabs[tabCounter].fields[i].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB1);
            tabs[tabCounter].fields[i].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, names[i], TAB1);
        }
        tabs[tabCounter].fields[3].counter = create_counter(tabs[tabCounter].tab, 0, 110, TAB1);
        tabs[tabCounter].fields[3].label = create_label(tabs[tabCounter].tab, 0, 110, names[3], TAB1);
        tabs[tabCounter].fields[3].bar = create_progress_bar(tabs[tabCounter].tab, 0, -230, 80, 0);
        lv_obj_set_width(tabs[tabCounter].fields[3].bar, 760);
        lv_obj_set_height(tabs[tabCounter].fields[3].bar, 60);
        
        
        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        
        lv_obj_add_style(tabs[tabCounter].fields[3].bar, &style_indic, LV_PART_INDICATOR);

        for(int i = 0; i<2; i++){
            xcoordinate = -285+180*i; ycoordinate = 180+40*i;
            tabs[tabCounter].fields[4+i].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB1);
            tabs[tabCounter].fields[4+i].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, names[4+i], TAB1);
        }
        for(int i = 0; i<2; i++){
            xcoordinate = 105+185*i; ycoordinate = 220-40*i;
            tabs[tabCounter].fields[6+i].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB1);
            tabs[tabCounter].fields[6+i].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, names[6+i], TAB1);
        }
        //lv_label_set_recolor(tabs[tabCounter].fields[VOLTAGE_INDEX].counter, true);
    }
    else{
        for(int i = 0; i<3; i++){
            for(int x = 0; x<3; x++){
                xcoordinate = -250+250*x; ycoordinate = -80+150*i;
                tabs[tabCounter].fields[3*i+x].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB2);
                tabs[tabCounter].fields[3*i+x].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, names[TAB1NUMFIELDS+3*i+x], TAB2);
                }
        }
        tabs[tabCounter].fields[9].bar = create_progress_bar(tabs[tabCounter].tab, 0, -230, 15000, 0);
        lv_obj_set_width(tabs[tabCounter].fields[9].bar, 760);
        lv_obj_set_height(tabs[tabCounter].fields[9].bar, 60);
        *(tabs[tabCounter].fields[9].value) = 7500;
        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        //lv_label_set_recolor(tabs[tabCounter].fields[TAB2_OIL_TEMPERATURE_INDEX].counter, true);
        //lv_label_set_recolor(tabs[tabCounter].fields[TAB2_OIL_PRESSURE_INDEX].counter, true);
        
        lv_obj_add_style(tabs[tabCounter].fields[9].bar, &style_indic, LV_PART_INDICATOR);
    }
}

void setTabView(lv_obj_t * e){
    tabview = e;
}

lv_obj_t * create_counter(lv_obj_t *parent, int x, int y, int tabNum){
    y-=5;
    lv_obj_t* counter = lv_label_create(parent);
    if(y==105 && !tabNum){
        LV_FONT_DECLARE(font_roboto_120_4bpp);
        lv_obj_set_style_text_font(counter, &font_roboto_120_4bpp, 0);
        y-=20;
    }
    else{
        LV_FONT_DECLARE(font_roboto_72_4bpp);
        lv_obj_set_style_text_font(counter, &font_roboto_72_4bpp, 0);
    }
    if(y<150 && !tabNum){
        char buffer[100] = ""; 
        sprintf(buffer, "#ffffff 12345");
        lv_label_set_text(counter, buffer);
    }
    else{
        lv_label_set_text(counter, "999");
    }
    lv_obj_align(counter, LV_ALIGN_CENTER, x, y-50);
    lv_label_set_recolor(counter, 1);
    return counter;
}

lv_obj_t * create_label(lv_obj_t *parent, int x, int y, char* name, int tabNum){
    lv_obj_t *label = lv_label_create(parent);
    LV_FONT_DECLARE(font_roboto_24_4bpp);
    lv_obj_set_style_text_font(label, &font_roboto_24_4bpp, 0);
    if(y<150 && !tabNum){  
        char buffer[100] = ""; 
        sprintf(buffer, "#ffffff %s", name);
        lv_label_set_recolor(label, 1);
        lv_label_set_text(label, buffer);
    }
    else{
        lv_label_set_text(label, name);
    }
    lv_obj_align(label, LV_ALIGN_CENTER, x, y-10);
    return label;
}

lv_obj_t * create_progress_bar(lv_obj_t *parent, int x, int y, float max, float min) {
    lv_obj_t *bar = lv_bar_create(parent);  // Create a bar
    lv_obj_set_size(bar, 200, 20); // Set bar size (width x height)
    lv_obj_align(bar, LV_ALIGN_CENTER, x, y+20); // Center the bar
    lv_bar_set_range(bar, min, max); // Set min-max range
    lv_bar_set_value(bar, (min+max)/2, LV_ANIM_OFF); // Initial value (50%)
    return bar;
}

void makeCircle(tab* tabs, int i){
    tabs[i].circle = lv_obj_create(tabs[i].tab);  // Create an object (base container)
    lv_obj_set_size(tabs[i].circle, 1300, 1300);           // Set width and height (should be equal for a perfect circle)
    lv_obj_set_style_radius(tabs[i].circle, LV_RADIUS_CIRCLE, 0); // Make it a circle
    lv_obj_set_style_bg_color(tabs[i].circle, lv_palette_main(LV_PALETTE_NONE), 0); // Set color
    lv_obj_align(tabs[i].circle, LV_ALIGN_CENTER, 0, -525);
    lv_obj_set_style_border_width(tabs[i].circle, 0, 0);

    lv_obj_move_background(tabs[i].circle);
    lv_obj_set_style_anim_time(tabs[i].circle, 0, 0);
}

void checkValueChange(tab tabs[], int tabNum, int index, double conversionFactor, int messageIndex1, int messageIndex2, twai_message_t message){
    if((!tabNum && index < 3) || (tabNum && index == TAB2_CELL_BALANCE_DELTA_INDEX)){
        if((*(tabs[tabNum].fields[index].value)) != ((message.data[messageIndex1] << 8 | message.data[messageIndex2])*conversionFactor)){
            *(tabs[tabNum].fields[index].value) = ((message.data[messageIndex1] << 8 | message.data[messageIndex2])*conversionFactor);
            updateObject(tabs[tabNum].fields[index], *(tabs[tabNum].fields[index].value), index, tabNum);
        }
    }

    if(messageIndex2){
        if((int)(*(tabs[tabNum].fields[index].value)) != (int)((message.data[messageIndex1] << 8 | message.data[messageIndex2])*conversionFactor)){
            *(tabs[tabNum].fields[index].value) = ((message.data[messageIndex1] << 8 | message.data[messageIndex2])*conversionFactor);
            updateObject(tabs[tabNum].fields[index], *(tabs[tabNum].fields[index].value), index, tabNum);
        }
    }
    else{
        if((int)(*(tabs[tabNum].fields[index].value)) != (int)((message.data[messageIndex1])*conversionFactor)){
            *(tabs[tabNum].fields[index].value) = ((message.data[messageIndex1])*conversionFactor);
            updateObject(tabs[tabNum].fields[index], *(tabs[tabNum].fields[index].value), index, tabNum);
        }
    }
}

void updateObject(field object, float value, int index, int page){
    char buffer[100] = "";
    if(page && index == TAB2_RPM_INDEX){
        lv_bar_set_value(object.bar, (int)value, LV_ANIM_OFF);
        lv_obj_invalidate(object.bar);
        return;
    }
    else if(!page && index == TAB1_RPM_INDEX){
        sprintf(buffer, " #ffffff %d ", (int)value);
        lv_label_set_text(object.counter, buffer);
        lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        lv_obj_invalidate(object.counter);
        lv_obj_invalidate(object.bar);
        return;
    }

    else if(!page && index < 3){
        sprintf(buffer, "#ffffff %.1f", value);
        lv_label_set_text(object.counter, buffer);
    }

    else if(page && index == TAB2_CELL_BALANCE_DELTA_INDEX){
        sprintf(buffer, "%.1f", value);
        lv_label_set_text(object.counter, buffer);
    }

    else{
        sprintf(buffer, "%d", (int)value);
        lv_label_set_text(object.counter, buffer);
    }

    lv_obj_invalidate(object.counter);
}

void updateArray(tab tabs[], twai_message_t message){
    switch(message.identifier){
        case 0x700:
            if(*(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) != (int)((message.data[2] << 8 | message.data[3])*REAR_BREAK_PRESSURE_CONVERSION)){
                if((int)((message.data[2] << 8 | message.data[3])*REAR_BREAK_PRESSURE_CONVERSION)>9000){
                    if(*(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) != 0){
                        *(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) = 0;
                        updateObject(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX], 0, TAB2_REAR_BREAK_PRESSURE_INDEX, TAB2);
                    }
                }
                else{
                    *(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) = (int)(message.data[2] << 8 | message.data[3])*REAR_BREAK_PRESSURE_CONVERSION;
                    updateObject(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX], (message.data[2] << 8 | message.data[3])*REAR_BREAK_PRESSURE_CONVERSION, TAB2_REAR_BREAK_PRESSURE_INDEX, TAB2);
                }
            }
            
            return;
        case 0x703:
            checkValueChange(tabs, TAB1, TAB1_MOTOR_TEMP_INDEX, MOTOR_TEMP_CONVERSION, 0, 1, message);
            checkValueChange(tabs, TAB1, TAB1_GL_VOLTAGE_INDEX, GL_VOLTAGE_CONVERSION, 2, 0, message);
            return;
        case 0x704:
            checkValueChange(tabs, TAB1, TAB1_PACK_VOLTAGE_INDEX, PACK_VOLTAGE_CONVERSION, 0, 1, message);
            checkValueChange(tabs, TAB1, TAB1_PACK_TEMP_INDEX, PACK_TEMP_CONVERSION, 2, 3, message);
            checkValueChange(tabs, TAB1, TAB1_INVERTER_TEMP_INDEX, INVERTER_TEMP_CONVERSION, 6, 7, message);
            return;
        case 0x6b1:
            checkValueChange(tabs, TAB1, TAB1_HIGHEST_CELL_TEMP_INDEX, HIGHEST_CELL_TEMP_CONVERSION, 4, 0, message);
            return;
        case 0x6b2:
            checkValueChange(tabs, TAB1, TAB1_LOWEST_CELL_VOLTAGE_INDEX, LOWEST_CELL_VOLTAGE_CONVERSION, 2, 3, message);
            if(*(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX].value) != (((message.data[2] << 8 | message.data[3])-(message.data[0] << 8 | message.data[1]))*CELL_BALANCE_DELTA_CONVERSION)){
                *(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX].value) = (((message.data[2] << 8 | message.data[3])-(message.data[0] << 8 | message.data[1]))*CELL_BALANCE_DELTA_CONVERSION);
                updateObject(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX], *(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX].value), TAB2_CELL_BALANCE_DELTA_INDEX, TAB2);
            }
            checkValueChange(tabs, TAB1, TAB1_RPM_INDEX, RPM_CONVERSION, 2, 3, message);
            checkValueChange(tabs, TAB2, TAB2_RPM_INDEX, RPM_CONVERSION, 2, 3, message);
            return;
    }
    // switch(message.identifier){
    //     case 0x700:
    //         if(*(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) != (int)((message.data[0] << 8 | message.data[1])*REAR_BREAK_PRESSURE_CONVERSION)){
    //             if((int)((message.data[0] << 8 | message.data[1])*REAR_BREAK_PRESSURE_CONVERSION)>9000){
    //                 if(*(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) != 0){
    //                     *(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) = 0;
    //                     updateObject(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX], 0, TAB2_REAR_BREAK_PRESSURE_INDEX, TAB2);
    //                 }
    //             }
    //             else{
    //                 *(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX].value) = (int)(message.data[0] << 8 | message.data[1])*REAR_BREAK_PRESSURE_CONVERSION;
    //                 updateObject(tabs[TAB2].fields[TAB2_REAR_BREAK_PRESSURE_INDEX], (message.data[0] << 8 | message.data[1])*REAR_BREAK_PRESSURE_CONVERSION, TAB2_REAR_BREAK_PRESSURE_INDEX, TAB2);
    //             }
    //         }
    //         return;
    //     case 0x703:
    //         checkValueChange(tabs, TAB1, TAB1_MOTOR_TEMP_INDEX, MOTOR_TEMP_CONVERSION, 0, 1, message);
    //         checkValueChange(tabs, TAB1, TAB1_GL_VOLTAGE_INDEX, GL_VOLTAGE_CONVERSION, 2, 3, message);
    //         return;
    //     case 0x704:
    //         checkValueChange(tabs, TAB1, TAB1_RPM_INDEX, RPM_CONVERSION, 0, 1, message);
    //         checkValueChange(tabs, TAB2, TAB2_RPM_INDEX, RPM_CONVERSION, 0, 1, message);
    //         checkValueChange(tabs, TAB1, TAB1_PACK_VOLTAGE_INDEX, PACK_VOLTAGE_CONVERSION, 2, 3, message);
    //         checkValueChange(tabs, TAB1, TAB1_PACK_TEMP_INDEX, PACK_TEMP_CONVERSION, 4, 5, message);
    //         checkValueChange(tabs, TAB1, TAB1_HIGHEST_CELL_TEMP_INDEX, HIGHEST_CELL_TEMP_CONVERSION, 6, 7, message);
    //         return;
    //     case 0x705:
    //         checkValueChange(tabs, TAB1, TAB1_INVERTER_TEMP_INDEX, INVERTER_TEMP_CONVERSION, 0, 1, message);
    //         checkValueChange(tabs, TAB2, TAB2_BRAKE_BIAS_INDEX, BRAKE_BIAS_CONVERSION, 2, 3, message);
    //         return;
    //     case 0x6b2:
    //         checkValueChange(tabs, TAB1, TAB1_LOWEST_CELL_VOLTAGE_INDEX, LOWEST_CELL_VOLTAGE_CONVERSION, 2, 3, message);
    //         if(*(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX].value) != (((message.data[2] << 8 | message.data[3])-(message.data[0] << 8 | message.data[1]))*CELL_BALANCE_DELTA_CONVERSION)){
    //             *(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX].value) = (((message.data[2] << 8 | message.data[3])-(message.data[0] << 8 | message.data[1]))*CELL_BALANCE_DELTA_CONVERSION);
    //             updateObject(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX], *(tabs[TAB2].fields[TAB2_CELL_BALANCE_DELTA_INDEX].value), TAB2_CELL_BALANCE_DELTA_INDEX, TAB2);
    //         }
    //         return;
    // }
        
    // if((WaterTempError || VoltageError || OilPressureError || OilTempError) && (goodZone != 0)){
    //     errorCode = 1;
    //     goodZone = 0;
    // }
    // else if(!(WaterTempError || VoltageError || OilPressureError || OilTempError)){
    //     errorCode = 0;
    //     goodZone = 1;
    // }need to set up. Use if tabs[index].value to check now that it is set
}

void warning(tab tabs[]){
    int changed = 0;
    while(true){
        if((errorCode && !goodZone) && (goodZone != errorCode)){
            changed = 1;
            lvgl_port_lock(-1);
            if(tab_id){
                switchTabID();
                switchTabView();
                lv_obj_invalidate(tabs[TAB1].tab);
            }
            lvgl_port_unlock();
            
            for(int i = 0; i<3 && (errorCode); i++){
                lvgl_port_lock(-1);
                lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_RED, 1), LV_PART_MAIN);
                lv_obj_set_style_bg_color(tabs[TAB1].circle, lv_palette_main(LV_PALETTE_BLUE), 0);
                lv_obj_invalidate(tabs[TAB1].tab);
                lvgl_port_unlock();
                vTaskDelay(200);
                
                lvgl_port_lock(-1);
                lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_BLUE, 1), LV_PART_MAIN);
                lv_obj_set_style_bg_color(tabs[TAB1].circle, lv_palette_main(LV_PALETTE_RED), 0);
                lv_obj_invalidate(tabs[TAB1].tab);
                lvgl_port_unlock();
                vTaskDelay(200);
                
                
            }
            errorCode = 0;
        }
        else if (changed){
            lvgl_port_lock(-1);
            lv_obj_set_style_bg_color(tabview, lv_palette_lighten(LV_PALETTE_YELLOW, 1), LV_PART_MAIN);
            lv_obj_set_style_bg_color(tabs[TAB1].circle, lv_palette_main(LV_PALETTE_NONE), 0);
            lv_obj_invalidate(tabs[tab_id].tab);
            lvgl_port_unlock();
            vTaskDelay(100);
            changed = 0;
        }
        else{
            vTaskDelay(100);
        }
    }
}

void switchTabID(int tabNum){
    tab_id = !tab_id;
}

void switchTabView(){
    lv_tabview_set_act(tabview, tab_id, LV_ANIM_OFF);
}
