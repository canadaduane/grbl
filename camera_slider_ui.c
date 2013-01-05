#include <avr/io.h>

#include "camera_slider_ui.h"
#include "print.h"
#include "motion_control.h"
#include "config.h"
#include "gcode.h"      // has to_millimeters
#include "protocol.h"
#include "report.h"
#include "nuts_bolts.h" // needed for cs_ui_display_state

volatile uint8_t cs_direction;
uint8_t cs_ui_motion_initiated;

void cs_ui_init()
{
  cs_direction = CS_DIR_NONE;
  cs_ui_motion_initiated = 0;

  // enable cs_analog_read (a2d) conversions
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
  sbi(ADCSRA, ADEN);
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

static char *move_left = "G1Z180";
static char *move_right = "G1Z0";

void cs_ui_motion_start()
{
  uint8_t dir = cs_direction;
  if (!cs_ui_motion_initiated) {
    cs_ui_motion_initiated = 1;

    cs_ui_apply_speed();

    switch(dir)
    {
      case CS_DIR_NONE:
        break;
      case CS_DIR_LEFT:
        report_status_message(protocol_execute_line(move_left));
        break;
      case CS_DIR_RIGHT:
        report_status_message(protocol_execute_line(move_right));
        break;
      default:
        printString("oh noes! (");
        printInteger((long)dir);
        printString(")\r\n");
    }    
  } else {
    // motion is either ongoing, or finished
    if (sys.state == STATE_IDLE) {
      // If finished, let the user control again
      cs_ui_motion_initiated = 0;
    }
  }
}

void cs_ui_apply_speed()
{
  long speed;
  
  speed = (long)cs_analog_read(2);
  gc.feed_rate = to_millimeters(speed*250/100+1);

  return ;
}

int cs_analog_read(uint8_t pin)
{
  uint8_t low, high;
  
#if defined(__AVR_ATmega32U4__)
  pin = analogPinToChannel(pin);
  ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#elif defined(ADCSRB) && defined(MUX5)
  // the MUX5 bit of ADCSRB selects whether we're reading from channels
  // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
  ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
  
  // set the analog reference (high two bits of ADMUX) and select the
  // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
  // to 0 (the default).
#if defined(ADMUX)
  ADMUX = (1 << 6) | (pin & 0x07);
#endif

  // start the conversion
  sbi(ADCSRA, ADSC);

  // ADSC is cleared when the conversion finishes
  while (bit_is_set(ADCSRA, ADSC));

  // we have to read ADCL first; doing so locks both ADCL
  // and ADCH until ADCH is read.  reading ADCL second would
  // cause the results of each conversion to be discarded,
  // as ADCL and ADCH would be locked when it completed.
  low  = ADCL;
  high = ADCH;

  // combine the two bytes
  return (high << 8) | low;
}
