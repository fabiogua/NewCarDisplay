// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: CarUi

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_Screen1
void ui_Screen1_screen_init(void);
void ui_event_Screen1(lv_event_t *e);
lv_obj_t *ui_Screen1;
lv_obj_t *ui_logoImage;

// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t *ui_Screen2;
lv_obj_t *ui_Label1;
lv_obj_t *ui_Bar1;
lv_obj_t *ui_Label2;
lv_obj_t *ui_Bar2;
lv_obj_t *ui_Label3;
lv_obj_t *ui_Bar3;
lv_obj_t *ui_Label4;
lv_obj_t *ui_Bar4;
lv_obj_t *ui_Arc1;
lv_obj_t *ui_Arc2;
lv_obj_t *ui_Label6;
lv_obj_t *ui_Label5;
lv_obj_t *ui_Panel2;
lv_obj_t *ui_Label8;
lv_obj_t *ui_Panel1;
lv_obj_t *ui_Label7;
lv_obj_t *ui_Bar6;
lv_obj_t *ui_Bar5;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP != 0
#error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Screen1(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
    {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_Screen2, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                              true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui_Screen2_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Screen1);
}
