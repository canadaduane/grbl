#ifndef camera_slider_ui_h
#define camera_slider_ui_h

#define CS_DIR_NONE  0
#define CS_DIR_LEFT  1
#define CS_DIR_RIGHT 2

extern volatile uint8_t cs_direction;

void cs_ui_init();

void cs_ui_display_state();

void cs_ui_motion_start();

void cs_ui_apply_speed();

int cs_analog_read(uint8_t pin);

#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#endif 