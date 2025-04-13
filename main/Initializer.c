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

void setUpFields(tab tabs[], int tabCounter, char* things[], float maxes[], float mins[]){
    tabs[tabCounter].fields = (field*)malloc(10*sizeof(field));
    for(int i = 0; i<10; i++){
        tabs[tabCounter].fields[i].value = (int*)malloc(sizeof(int));
    }

    int xcoordinate, ycoordinate;
    if(!tabCounter){
        for(int i = 0; i<3; i++){
            xcoordinate = -270+270*i; ycoordinate = -65+abs(-30+30*i);
            tabs[tabCounter].fields[i].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB1);
            tabs[tabCounter].fields[i].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, things[i], TAB1);
            //tabs[tabCounter].fields[i].bar = create_progress_bar(tabs[tabCounter].tab, xcoordinate, ycoordinate, maxes[i], mins[i]);
            *(tabs[tabCounter].fields[i].value) = (maxes[i]+mins[i])/2;
        }
        tabs[tabCounter].fields[3].counter = create_counter(tabs[tabCounter].tab, 0, 110, TAB1);
        tabs[tabCounter].fields[3].label = create_label(tabs[tabCounter].tab, 0, 110, things[3], TAB1);
        tabs[tabCounter].fields[3].bar = create_progress_bar(tabs[tabCounter].tab, 0, -230, maxes[3], mins[3]);
        lv_obj_set_width(tabs[tabCounter].fields[3].bar, 760);
        lv_obj_set_height(tabs[tabCounter].fields[3].bar, 60);
        *(tabs[tabCounter].fields[3].value) = (maxes[3]+mins[3])/2;
        
        
        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        //lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
        // lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
        // lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);
        
        lv_obj_add_style(tabs[tabCounter].fields[3].bar, &style_indic, LV_PART_INDICATOR);
        // lv_obj_set_style_bg_color(tabs[tabCounter].fields[3].bar, lv_palette_main(LV_PALETTE_GREEN), 0);
        // lv_obj_set_style_bg_opa(tabs[tabCounter].fields[3].bar, 0, 0);

        for(int i = 0; i<2; i++){
            xcoordinate = -285+180*i; ycoordinate = 180+40*i;
            tabs[tabCounter].fields[4+i].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB1);
            tabs[tabCounter].fields[4+i].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, things[4+i], TAB1);
            *(tabs[tabCounter].fields[4+i].value) = (maxes[4+i]+mins[4+i])/2;
        }
        for(int i = 0; i<2; i++){
            xcoordinate = 105+185*i; ycoordinate = 220-40*i;
            tabs[tabCounter].fields[6+i].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB1);
            tabs[tabCounter].fields[6+i].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, things[6+i], TAB1);
            *(tabs[tabCounter].fields[6+i].value) = (maxes[6+i]+mins[6+i])/2;
        }
        lv_label_set_recolor(tabs[tabCounter].fields[VOLTAGE_INDEX].counter, true);
    }
    else{
        for(int i = 0; i<3; i++){
            for(int x = 0; x<3; x++){
                xcoordinate = -250+250*x; ycoordinate = -80+150*i;
                tabs[tabCounter].fields[3*i+x].counter = create_counter(tabs[tabCounter].tab, xcoordinate, ycoordinate, TAB2);
                tabs[tabCounter].fields[3*i+x].label = create_label(tabs[tabCounter].tab, xcoordinate, ycoordinate, things[TAB1NUMFIELDS+3*i+x], TAB2);
                //tabs[tabCounter].fields[3*i+x].bar = create_progress_bar(tabs[tabCounter].tab, xcoordinate, ycoordinate, maxes[TAB1NUMFIELDS+3*i+x], mins[TAB1NUMFIELDS+3*i+x]);
            }
        }
        tabs[tabCounter].fields[9].bar = create_progress_bar(tabs[tabCounter].tab, 0, -230, 15000, 0);
        lv_obj_set_width(tabs[tabCounter].fields[9].bar, 760);
        lv_obj_set_height(tabs[tabCounter].fields[9].bar, 60);
        *(tabs[tabCounter].fields[9].value) = 7500;
        lv_style_init(&style_indic);
        lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
        lv_label_set_recolor(tabs[tabCounter].fields[TAB2_OIL_TEMPERATURE_INDEX].counter, true);
        lv_label_set_recolor(tabs[tabCounter].fields[TAB2_OIL_PRESSURE_INDEX].counter, true);
        //lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
        // lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
        // lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_HOR);
        
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
        lv_label_set_recolor(counter, 1);
        lv_label_set_text(counter, buffer);
    }
    else{
        lv_label_set_text(counter, "999");
    }
    lv_obj_align(counter, LV_ALIGN_CENTER, x, y-50);
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
}

void updateObject(field object, int value, int index, int page){
    // if(page && index == LAMBDA_INDEX){
    //     char buffer[100] = ""; 
    //     sprintf(buffer, "%.2f", value*LAMBDA_CONVERSION);
    //     lv_label_set_text(object.counter, buffer);
    //     lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
    //     return;
    // }
    // if(page && (index == TAB2_OIL_TEMPERATURE_INDEX  || index == TAB2_OIL_PRESSURE_INDEX)){
    //     char buffer[100] = ""; 
    //     sprintf(buffer, "%d", value);
    //     lv_label_set_text(object.counter, buffer);
    //     return;
    // }

    if(page && index == TAB2_RPM_INDEX){
        lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        lv_obj_invalidate(object.bar);
        return;
    }

    if(page && index == TAB2_OIL_TEMPERATURE_INDEX && OilTempError){
        char buffer[100] = "";
        sprintf(buffer, "#ff0000 %d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        return;
    }

    if(page && index == TAB2_OIL_PRESSURE_INDEX && OilPressureError){
        char buffer[100] = "";
        sprintf(buffer, "#ff0000 %d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        return;
    }

    if(index == GEAR_POSITION_SOURCE_INDEX && page){
        switch (value){
            case 0:
                lv_label_set_text(object.counter, "S");
                //lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
                break;
            case 1:
                lv_label_set_text(object.counter, "E");
                //lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
                break;
            case 2:
                lv_label_set_text(object.counter, "D");
                //lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
                break;
        }
        lv_obj_invalidate(object.counter);
        return;
    }

    if(index == FRONT_BRAKE_PRESSURE_INDEX && value > 9000 && page){
        lv_label_set_text(object.counter, "0");
        lv_obj_invalidate(object.counter);
        return;
    }

    if(page){
        char buffer[100] = ""; 
        sprintf(buffer, "%d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        return;
    }
    
    if(WaterTempError && index == WATER_TEMP_INDEX){
        char buffer[100] = ""; 
        sprintf(buffer, "#ff0000 %d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        //lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        return;
    }
    
    if(VoltageError && index == VOLTAGE_INDEX){
        char buffer[100] = ""; 
        sprintf(buffer, "#ff0000 %.1f", ((float)value) / 10);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        return;
    }

    if(OilPressureError && index == OIL_PRESSURE_INDEX){
        char buffer[100] = ""; 
        sprintf(buffer, "#ff0000 %d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        // lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        return;
    }

    if(OilTempError && (index == OIL_TEMP_INDEX || index == TAB2_OIL_TEMPERATURE_INDEX)){
        char buffer[100] = "";
        sprintf(buffer, "#ff0000 %d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        return;
    }


    if(index == VOLTAGE_INDEX){
        char buffer[100] = "";
        sprintf(buffer, "%.1f", ((float)value)/10);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        return;
    }
    char buffer[100] = "";
    sprintf(buffer, "%d", value);

    if(index == GEAR_INDEX && value == 0){
        lv_label_set_text(object.counter, "N");
        lv_obj_invalidate(object.counter);
        return;
    }

    if(index == GEAR_INDEX && value > 6){
        lv_label_set_text(object.counter, "D");
        lv_obj_invalidate(object.counter);
        return;
    }

    if(index == RPM_INDEX){
        char buffer[100] = ""; 
        sprintf(buffer, " #ffffff %d ", value);
        lv_label_set_text(object.counter, buffer);
        lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        lv_obj_invalidate(object.counter);
        lv_obj_invalidate(object.bar);
        return;
    }

    else if((index == WATER_TEMP_INDEX || index == OIL_PRESSURE_INDEX || index == OIL_TEMP_INDEX)){
        char buffer[100] = ""; 
        sprintf(buffer, "#ffffff %d", value);
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        //lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        return;
    }
    else{
        lv_label_set_text(object.counter, buffer);
        lv_obj_invalidate(object.counter);
        //lv_bar_set_value(object.bar, value, LV_ANIM_OFF);
        return;
    }

}

void updateArray(tab tabs[]){
    //while(true){
        twai_message_t message = recieve_CAN();
        if((message.identifier/0x100 != 0x7)&&(message.identifier/0x100 !=0x3)) {
            return;
        }
            //printf("madeit\n");
        
        switch(message.identifier){
            case 0x700:

                if(*(tabs[TAB1].fields[MPH_INDEX].value) != (message.data[0] << 8 | message.data[1])){
                    *(tabs[TAB1].fields[MPH_INDEX].value) = (message.data[0] << 8 | message.data[1]);
                    updateObject(tabs[TAB1].fields[MPH_INDEX], (message.data[0] << 8 | message.data[1])/10, MPH_INDEX, TAB1);
                }

                if(*(tabs[TAB1].fields[RPM_INDEX].value) != (message.data[2] << 8 | message.data[3])){
                    *(tabs[TAB1].fields[RPM_INDEX].value) = (message.data[2] << 8 | message.data[3]);
                    if((message.data[2] << 8 | message.data[3])<7500){
                        lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
                    }
                    else if((message.data[2] << 8 | message.data[3])<12000){
                        lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_ORANGE));
                    }
                    else{
                        lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
                    }
                    updateObject(tabs[TAB1].fields[RPM_INDEX], (message.data[2] << 8 | message.data[3]), RPM_INDEX, TAB1);
                    updateObject(tabs[TAB2].fields[TAB2_RPM_INDEX], (message.data[2] << 8 | message.data[3]), TAB2_RPM_INDEX, TAB2);
                }

                //lv_obj_invalidate(tabs[0].tab);
                if(*(tabs[TAB1].fields[VOLTAGE_INDEX].value) != (message.data[4])){
                    *(tabs[TAB1].fields[VOLTAGE_INDEX].value) = (message.data[4]);
                    if((message.data[4]/10)<10){
                        VoltageError = 1;
                    }
                    else{
                        VoltageError = 0;
                    }
                    updateObject(tabs[TAB1].fields[VOLTAGE_INDEX], (message.data[4]), VOLTAGE_INDEX, TAB1);
                }

                if(*(tabs[TAB1].fields[WATER_TEMP_INDEX].value) != (int)((message.data[5] << 8 | message.data[6])*WATER_TEMP_CONVERSION)){
                    *(tabs[TAB1].fields[WATER_TEMP_INDEX].value) = (message.data[5] << 8 | message.data[6])*WATER_TEMP_CONVERSION;
                    if((message.data[5] << 8 | message.data[6])*WATER_TEMP_CONVERSION>225){
                        WaterTempError = 1;
                    }
                    else{
                        WaterTempError = 0;
                    }
                    updateObject(tabs[TAB1].fields[WATER_TEMP_INDEX], (message.data[5] << 8 | message.data[6])*WATER_TEMP_CONVERSION, WATER_TEMP_INDEX, TAB1);
                }

                if(*(tabs[TAB1].fields[FUEL_PRESSURE_INDEX].value) != message.data[7]){
                    *(tabs[TAB1].fields[FUEL_PRESSURE_INDEX].value) = message.data[7];
                    updateObject(tabs[TAB1].fields[FUEL_PRESSURE_INDEX], (message.data[7]), FUEL_PRESSURE_INDEX, TAB1);
                }

                

                //lv_obj_invalidate(tabs[0].tab);
                break;
            case 0x701:

                if(*(tabs[TAB1].fields[OIL_PRESSURE_INDEX].value) != (int)((message.data[0] << 8 | message.data[1])*OIL_PRESSURE_CONVERSION)){
                    *(tabs[TAB1].fields[OIL_PRESSURE_INDEX].value) = (message.data[0] << 8 | message.data[1])*OIL_PRESSURE_CONVERSION;
                    if(((message.data[0] << 8 | message.data[1])*OIL_PRESSURE_CONVERSION)<10 || ((message.data[0] << 8 | message.data[1])*OIL_PRESSURE_CONVERSION)>95){ 
                        OilPressureError = 1;
                    }
                    else{
                        OilPressureError = 0;
                    }
                    updateObject(tabs[TAB1].fields[OIL_PRESSURE_INDEX], (message.data[0] << 8 | message.data[1])*OIL_PRESSURE_CONVERSION, OIL_PRESSURE_INDEX, TAB1);
                    updateObject(tabs[TAB2].fields[TAB2_OIL_PRESSURE_INDEX], (message.data[0] << 8 | message.data[1])*OIL_PRESSURE_CONVERSION, TAB2_OIL_PRESSURE_INDEX, TAB2);

                }                

                if(*(tabs[TAB2].fields[ABSOLUTE_MANIFOLD_PRESSURE_INDEX].value) != (int)((message.data[2] << 8 | message.data[3])*ABSOLUTE_MANIFOLD_PRESSURE_CONVERSION)){
                    *(tabs[TAB2].fields[ABSOLUTE_MANIFOLD_PRESSURE_INDEX].value) = ((message.data[2] << 8 | message.data[3])*ABSOLUTE_MANIFOLD_PRESSURE_CONVERSION);
                    updateObject(tabs[TAB2].fields[ABSOLUTE_MANIFOLD_PRESSURE_INDEX], (message.data[2] << 8 | message.data[3])*ABSOLUTE_MANIFOLD_PRESSURE_CONVERSION, ABSOLUTE_MANIFOLD_PRESSURE_INDEX, TAB2);
                }

                if(*(tabs[TAB2].fields[LAMBDA_INDEX].value) != ((message.data[6])*LAMBDA_CONVERSION)){
                    *(tabs[TAB2].fields[LAMBDA_INDEX].value) = ((message.data[6])*LAMBDA_CONVERSION);
                    updateObject(tabs[TAB2].fields[LAMBDA_INDEX], (message.data[6])*LAMBDA_CONVERSION, LAMBDA_INDEX, TAB2);
                }
                break;
            case 0x702:

                if(*(tabs[TAB2].fields[FRONT_BRAKE_PRESSURE_INDEX].value) != (int)((message.data[4] << 8 | message.data[5])*FRONT_BRAKE_PRESSURE_CONVERSION)){
                    *(tabs[TAB2].fields[FRONT_BRAKE_PRESSURE_INDEX].value) = (message.data[4] << 8 | message.data[5])*FRONT_BRAKE_PRESSURE_CONVERSION;
                    updateObject(tabs[TAB2].fields[FRONT_BRAKE_PRESSURE_INDEX], (message.data[4] << 8 | message.data[5])*FRONT_BRAKE_PRESSURE_CONVERSION, FRONT_BRAKE_PRESSURE_INDEX, TAB2);
                }

                return;
            case 0x704:

                if(*(tabs[TAB1].fields[GEAR_INDEX].value) != ((message.data[2]))){
                    *(tabs[TAB1].fields[GEAR_INDEX].value) = ((message.data[2]));
                    updateObject(tabs[TAB1].fields[GEAR_INDEX], (message.data[2]), GEAR_INDEX, TAB1);
                }
                if(*(tabs[TAB1].fields[OIL_TEMP_INDEX].value) != (int)((message.data[6] << 8 | message.data[7])*OIL_TEMP_CONVERSION)){
                    *(tabs[TAB1].fields[OIL_TEMP_INDEX].value) = ((message.data[6] << 8 | message.data[7])*OIL_TEMP_CONVERSION);
                    if(((message.data[6] << 8 | message.data[7])*OIL_TEMP_CONVERSION)>240){
                        OilTempError = 1;
                    }
                    else{
                        OilTempError = 0;
                    }
                    updateObject(tabs[TAB1].fields[OIL_TEMP_INDEX], (message.data[6] << 8 | message.data[7])*OIL_TEMP_CONVERSION, OIL_TEMP_INDEX, TAB1);
                    updateObject(tabs[TAB2].fields[TAB2_OIL_TEMPERATURE_INDEX], (message.data[6] << 8 | message.data[7])*OIL_TEMP_CONVERSION, TAB2_OIL_TEMPERATURE_INDEX, TAB2);
                }

                // if(*(tabs[TAB1].fields[RPM_INDEX].value) != ((message.data[0]) << 8 | message.data[1])){
                //     *(tabs[TAB1].fields[RPM_INDEX].value) = ((message.data[0]) << 8 | message.data[1]);
                //     updateObject(tabs[TAB1].fields[RPM_INDEX], ((message.data[0]) << 8 | message.data[1]), RPM_INDEX, TAB1);
                //     screenChange = true;
                // }

                break;
            case 0x70f:
                if(*(tabs[TAB2].fields[GEAR_POSITION_SOURCE_INDEX].value) != ((message.data[4]))){
                    *(tabs[TAB2].fields[GEAR_POSITION_SOURCE_INDEX].value) = ((message.data[4]));
                    updateObject(tabs[TAB2].fields[GEAR_POSITION_SOURCE_INDEX], (message.data[4]), GEAR_POSITION_SOURCE_INDEX, TAB2);
                }
                return;
            case 0x714:
                if(*(tabs[TAB2].fields[TANK_PRESSURE_INDEX].value) != (int)((message.data[0] << 8 | message.data[1])*TANK_PRESSURE_CONVERSION)){
                    *(tabs[TAB2].fields[TANK_PRESSURE_INDEX].value) = ((message.data[0] << 8 | message.data[1])*TANK_PRESSURE_CONVERSION);
                    updateObject(tabs[TAB2].fields[TANK_PRESSURE_INDEX], (message.data[0] << 8 | message.data[1])*TANK_PRESSURE_CONVERSION, TANK_PRESSURE_INDEX, TAB2);
                }

                if(*(tabs[TAB2].fields[REGULATOR_PRESSURE_INDEX].value) != (int)((message.data[2] << 8 | message.data[3])*REGULATOR_PRESSURE_CONVERSION)){
                    *(tabs[TAB2].fields[REGULATOR_PRESSURE_INDEX].value) = (message.data[2] << 8 | message.data[3])*REGULATOR_PRESSURE_CONVERSION;
                    updateObject(tabs[TAB2].fields[REGULATOR_PRESSURE_INDEX], (message.data[2] << 8 | message.data[3])*REGULATOR_PRESSURE_CONVERSION, REGULATOR_PRESSURE_INDEX, TAB2);
                }

                if(*(tabs[TAB2].fields[BRAKE_BIAS_INDEX].value) != (message.data[4] << 8 | message.data[5])){
                    *(tabs[TAB2].fields[BRAKE_BIAS_INDEX].value) = (message.data[4] << 8 | message.data[5]);
                    updateObject(tabs[TAB2].fields[BRAKE_BIAS_INDEX], (message.data[4] << 8 | message.data[5]), BRAKE_BIAS_INDEX, TAB2);
                }

                // if(screenChange){
                //     lvgl_port_lock(-1);
                //     lv_obj_invalidate(tabs[tab_id].tab);
                //     lvgl_port_unlock();
                //     screenChange = 0;
                // }
                return;
        }
        
        if((WaterTempError || VoltageError || OilPressureError || OilTempError) && (goodZone != 0)){
            errorCode = 1;
            goodZone = 0;
        }
        else if(!(WaterTempError || VoltageError || OilPressureError || OilTempError)){
            errorCode = 0;
            goodZone = 1;
        }
        
    //}
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
            vTaskDelay(500);
            changed = 0;
        }
        else{
            vTaskDelay(500);//#include<unistd.h> usleep();
        }
    }
}

void switchTabID(int tabNum){
    tab_id = !tab_id;
}

void switchTabView(){
    lv_tabview_set_act(tabview, tab_id, LV_ANIM_OFF);
}

// void button_init() {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << 6), // Select GPIO 6
//         .mode = GPIO_MODE_INPUT,               // Set as input mode
//         .pull_up_en = GPIO_PULLUP_ENABLE,      // Enable pull-up (if needed)
//         .pull_down_en = GPIO_PULLDOWN_DISABLE, // Disable pull-down
//         .intr_type = GPIO_INTR_NEGEDGE         // Interrupt on falling edge (adjust as needed)
//     };
//     gpio_config(&io_conf);
// }

// esp_err_t SD_init(sdmmc_slot_config_t* SD_config){
//     const char mountpoint[] = MOUNT_POINT;
//     sdmmc_host_t host = SDMMC_HOST_DEFAULT();
//     //host.max_freq_khz = SD_BUS_FREQ;
//     esp_vfs_fat_mount_config_t mount_config = VFS_FAT_MOUNT_DEFAULT_CONFIG();
//     mount_config.max_files = 10;
//     mount_config.format_if_mount_failed = false;
//     mount_config.allocation_unit_size = 16 * 1024;
//     ESP_ERROR_CHECK(esp_vfs_fat_sdmmc_mount(mountpoint, &host, SD_config, &mount_config, NULL));
//     ESP_LOGI(TAG, "SD card has been initialized");
//     return ESP_OK;
// }

