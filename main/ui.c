#include "lvgl/lvgl.h"
#include "lvgl_helpers.h"

void lv_example_style_1(void)
{
    lv_obj_t *btn = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 100);                 /*Set its position*/
    lv_obj_set_size(btn, 120, 50);               /*Set its size*/
    // lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t *label = lv_label_create(btn);  /*Add a label to the button*/
    lv_label_set_text(label, "Wo0000y"); /*Set the labels text*/
    lv_obj_center(label);
}

void lv_example_style_2(void)
{
    lv_obj_t *btn = lv_btn_create(lv_scr_act()); /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                 /*Set its position*/
    lv_obj_set_size(btn, 120, 50);               /*Set its size*/
    // lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t *label = lv_label_create(btn);  /*Add a label to the button*/
    lv_label_set_text(label, "EMei_Li"); /*Set the labels text*/
    lv_obj_center(label);
}