#include <avr/io.h>

#include "camera_slider_ui.h"
#include "print.h"
#include "motion_control.h"
#include "config.h"

uint8_t cs_direction;

void cs_ui_init()
{
  cs_direction = CS_DIR_NONE;
}

void cs_ui_motion_start()
{
  printString("motion start\r\n");
  switch(cs_direction)
  {
    case CS_DIR_NONE:
      printString("no movement\r\n");
      break;
    case CS_DIR_LEFT:
      printString("move left\r\n");
      mc_line(0, 0, 0, 1000, 0);
      break;
    case CS_DIR_RIGHT:
      printString("move right\r\n");
      mc_line(0, 0, 100, 1000, 0);
      break;
    default:
      printString("oh noes!\r\n");
  }
}

void cs_ui_set_direction(uint8_t dir)
{
  cs_direction = dir;
}
