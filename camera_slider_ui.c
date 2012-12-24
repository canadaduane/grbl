#include <avr/io.h>

#include "camera_slider_ui.h"
#include "print.h"
#include "motion_control.h"
#include "config.h"
#include "gcode.h"      // needed for cs_ui_motion_start
#include "nuts_bolts.h" // needed for cs_ui_display_state

uint8_t cs_direction;

void cs_ui_init()
{
  cs_direction = CS_DIR_NONE;
}

void cs_ui_display_state() {
  printString("State: ");
  switch (sys.state)
  {
    case STATE_IDLE:       printString("IDLE");       break;
    case STATE_QUEUED:     printString("QUEUED");     break;
    case STATE_CYCLE:      printString("CYCLE");      break;
    case STATE_HOLD:       printString("HOLD");       break;
    case STATE_HOMING:     printString("HOMING");     break;
    case STATE_ALARM:      printString("ALARM");      break;
    case STATE_CHECK_MODE: printString("CHECK_MODE"); break;
    default: printString("UNKNOWN");
  }
  printString("\r\n");
}

void cs_ui_motion_start()
{
  printString("motion start\r\n");
  cs_ui_display_state();
  switch(cs_direction)
  {
    case CS_DIR_NONE:
      printString("no movement\r\n");
      break;
    case CS_DIR_LEFT:
      printString("move left\r\n");
      // gc.status_code = STATUS_OK;
      // mc_line(0, 0, 0, 1000, 0);
      gc_execute_line("G1 Z100 F1500");
      break;
    case CS_DIR_RIGHT:
      printString("move right\r\n");
      // mc_line(0, 0, 100, 1000, 0);
      gc_execute_line("G1 Z0 F1500");
      break;
    default:
      printString("oh noes!\r\n");
  }
}

void cs_ui_set_direction(uint8_t dir)
{
  cs_direction = dir;
}
