#include "lvgl/lvgl.h"
// #include "lvgl_helpers.h"
#include "stdio.h"

typedef struct {
    lv_obj_t * arc_sec;
    lv_obj_t * arc_min;
    lv_obj_t * arc_hour;
    lv_obj_t * time_lable;

    lv_style_t sec_style, arc_bg;
    lv_style_t min_style;
    lv_style_t hour_style;

    uint8_t sec, min, hour;

    void (*timer_cb)(void);
} time_ui_t;

static time_ui_t time_ui;

static void set_angle(void * obj, int32_t v)
{
    char str[20];

    if (v == 100) {
        time_ui.sec += 1;
        if (time_ui.sec == 60) {
            time_ui.sec = 0;
            time_ui.min += 1;
            if (time_ui.min == 60) {
                time_ui.min = 0;
                time_ui.hour += 1;
                if (time_ui.hour == 24) {
                    time_ui.hour = 0;
                }
            }
        }

        sprintf(str, "%02u:%02u:%02u", time_ui.hour, time_ui.min, time_ui.sec);
        lv_label_set_text(time_ui.time_lable, str);
    }
    lv_arc_set_value(time_ui.arc_sec, v);
    lv_arc_set_value(time_ui.arc_min, time_ui.min*100/60);
    lv_arc_set_value(time_ui.arc_hour, (time_ui.hour%12)*100/12);
}

void init_time_ui(void)
{
    time_ui.hour = 10;
    time_ui.min = 16;

    lv_style_init(&time_ui.sec_style);
    lv_style_init(&time_ui.min_style);
    lv_style_init(&time_ui.hour_style);

    lv_style_init(&time_ui.arc_bg);

    time_ui.arc_sec  = lv_arc_create(lv_scr_act());
    time_ui.arc_min  = lv_arc_create(lv_scr_act());
    time_ui.arc_hour = lv_arc_create(lv_scr_act());

    time_ui.time_lable = lv_label_create(lv_scr_act());
    lv_obj_set_width(time_ui.time_lable, 65);
    lv_label_set_text(time_ui.time_lable, "--:--:--");
    //LV_FONT_MONTSERRAT_12
    lv_obj_set_style_text_font(time_ui.time_lable, &lv_font_montserrat_16, 0);
    lv_obj_align(time_ui.time_lable, LV_ALIGN_CENTER, 2, 0);

    lv_obj_set_size(time_ui.arc_sec, 180, 180);
    lv_obj_set_size(time_ui.arc_min, 150, 150);
    lv_obj_set_size(time_ui.arc_hour, 120, 120);

    lv_style_set_arc_width(&time_ui.arc_bg, 13);
    lv_obj_add_style(time_ui.arc_sec, &time_ui.arc_bg, 0);
    lv_obj_add_style(time_ui.arc_min, &time_ui.arc_bg, 0);
    lv_obj_add_style(time_ui.arc_hour, &time_ui.arc_bg, 0);



    lv_style_set_arc_width(&time_ui.sec_style, 13);
    lv_style_set_arc_color(&time_ui.sec_style, lv_palette_main(LV_PALETTE_ORANGE));
    lv_obj_add_style(time_ui.arc_sec, &time_ui.sec_style, LV_PART_INDICATOR);
    lv_arc_set_rotation(time_ui.arc_sec, 270);
    lv_arc_set_bg_angles(time_ui.arc_sec, 0, 360);
    lv_obj_remove_style(time_ui.arc_sec, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(time_ui.arc_sec, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(time_ui.arc_sec);
    lv_arc_set_value(time_ui.arc_sec, 0);


    lv_style_set_arc_width(&time_ui.min_style, 13);
    lv_style_set_arc_color(&time_ui.min_style, lv_palette_main(LV_PALETTE_TEAL));
    lv_obj_add_style(time_ui.arc_min, &time_ui.min_style, LV_PART_INDICATOR);
    lv_arc_set_rotation(time_ui.arc_min, 270);
    lv_arc_set_bg_angles(time_ui.arc_min, 0, 360);
    lv_obj_remove_style(time_ui.arc_min, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(time_ui.arc_min, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(time_ui.arc_min);
    lv_arc_set_value(time_ui.arc_min, 0);

    lv_style_set_arc_width(&time_ui.hour_style, 13);
    lv_style_set_arc_color(&time_ui.hour_style, lv_palette_main(LV_PALETTE_PURPLE));
    lv_obj_add_style(time_ui.arc_hour, &time_ui.hour_style, LV_PART_INDICATOR);
    lv_arc_set_rotation(time_ui.arc_hour, 270);
    lv_arc_set_bg_angles(time_ui.arc_hour, 0, 360);
    lv_obj_remove_style(time_ui.arc_hour, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(time_ui.arc_hour, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_center(time_ui.arc_hour);
    lv_arc_set_value(time_ui.arc_hour, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, time_ui.arc_sec);
    lv_anim_set_exec_cb(&a, set_angle);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&a, 0);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_start(&a);
}


void lv_example_style_1(void)
{
    // lv_obj_t * arc = lv_arc_create(lv_scr_act());
    // lv_obj_set_size(arc, 150, 150);
    // lv_arc_set_rotation(arc, 135);
    // lv_arc_set_bg_angles(arc, 0, 270);
    // lv_arc_set_value(arc, 40);
    // lv_obj_center(arc);
    init_time_ui();
}



void lv_example_style_2(void)
{
    // static lv_style_t style;
    // lv_style_init(&style);

    // /*Set a background color and a radius*/
    // lv_style_set_radius(&style, 5);
    // lv_style_set_bg_opa(&style, LV_OPA_COVER);
    // lv_style_set_bg_color(&style, lv_palette_lighten(LV_PALETTE_GREY, 1));

    // /*Add outline*/
    // lv_style_set_outline_width(&style, 2);
    // lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_BLUE));
    // lv_style_set_outline_pad(&style, 8);

    // /*Create an object with the new style*/
    // lv_obj_t * obj = lv_obj_create(lv_scr_act());
    // lv_obj_add_style(obj, &style, 0);
    // lv_obj_center(obj);

    
    // lv_obj_t * arc = lv_arc_create(lv_scr_act());
    // // static lv_style_t style;
    // // lv_style_init(&style);
    // // lv_style_set_arc_color(&style, lv_palette_main(LV_PALETTE_ORANGE));
    // // lv_style_set_arc_width(&style, 10);
    // // lv_obj_add_style(arc, &style, 0);
    // lv_obj_set_size(arc, 150, 150);
    // lv_arc_set_rotation(arc, 270);
    // lv_arc_set_bg_angles(arc, 0, 360);
    // lv_obj_remove_style(arc, NULL, LV_PART_KNOB);   /*Be sure the knob is not displayed*/
    // lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);  /*To not allow adjusting by click*/
    // lv_obj_center(arc);

    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_var(&a, arc);
    // lv_anim_set_exec_cb(&a, set_angle);
    // lv_anim_set_time(&a, 1500);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);    /*Just for the demo*/
    // lv_anim_set_repeat_delay(&a, 500);
    // lv_anim_set_values(&a, 0, 100);
    // lv_anim_start(&a);
}