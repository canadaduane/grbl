#ifndef camera_slider_ui_h
#define camera_slider_ui_h

#define CS_DIR_NONE  0
#define CS_DIR_LEFT  1
#define CS_DIR_RIGHT 2

extern uint8_t cs_direction;

void cs_ui_init();

void cs_ui_motion_start();

void cs_ui_set_direction(uint8_t dir);

void cs_ui_get_speed();

#endif 