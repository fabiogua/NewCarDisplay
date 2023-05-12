/**
 * Notebook page https://os.mbed.com/users/JohnnyK/notebook/how-start-with-the-LVGL/
 */

#include "mbed.h"
#include "../lvgl.h"
// #include "demos/lv_demos.h" /*Comment/uncomment will switch between LVGL demos and Hello word example*/
#include "../lib/hal_stm_lvgl/touchpad/touchpad.h"
#include "../lib/hal_stm_lvgl/tft/tft.h"

#include "ui_files/ui.h"

#define LVGL_TICK 10     // Time tick value for lvgl in ms (1-10msa)
#define TICKER_TIME 10ms // modified to miliseconds

Ticker ticker; // Ticker for lvgl

/*
 * Callback function for lvgl timing.
 * Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10).
 * It is required for the internal timing of LittlevGL.
 */
void lv_ticker_func()
{
    lv_tick_inc(LVGL_TICK);
}

#ifndef LV_DEMOS_H
static void event_handler(lv_event_t *event)
{
    lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CLICKED)
    {
        printf("Clicked\n");
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        printf("Toggled\n");
    }
}
#endif
// main() runs in its own thread in the OS
int main()
{
    printf("LVGL-");
    lv_init();                                             // Initialize the LVGL
    tft_init();                                            // Initialize diplay
    touchpad_init();                                       // Initialize touchpad
    ticker.attach(callback(&lv_ticker_func), TICKER_TIME); // Attach callback to ticker

#ifdef LV_DEMOS_H
    printf("Demo\n");
    lv_demo_widgets();
#endif
#ifdef TEST_LV

    printf("Hello world\n");
    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP); /*Break the long lines*/
    lv_label_set_recolor(label1, true);                 /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "#0000ff Hello# #ff00ff world# #ff0000 - the LVGL and MbedOS#");
    lv_obj_set_width(label1, 150);
    lv_obj_align(label1, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t *label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_set_width(label2, 150);
    lv_label_set_text(label2, "It is a circularly scrolling text. ");
    lv_obj_align(label2, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *label;
    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -25);
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 25);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggled");
#else
    printf("UI\n");
    ui_init();

#endif

    while (true)
    {
        lv_task_handler();
        // Call lv_task_handler() periodically every few milliseconds.
        // It will redraw the screen if required, handle input devices etc.
        thread_sleep_for(LVGL_TICK);
    }
}
