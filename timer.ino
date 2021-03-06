#include <avr/io.h>
#include <LiquidCrystal_I2C.h>
#include <avr/interrupt.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
short int mode = 0;           //0=setup; 1=counting; 2=alarm
int h = 0;                    //hours
int m = 0;                    //minutes
int s = 0;                    //seconds
char _time[5];                //array with time
short int debounceDelay = 20; //debounce time
short int cursorPos = 11;     //cursor position
short int blinkDelay = 50;    //used to blink a LED
bool LED = 0;                 //current LED state
int s_cpy = 0;                //copy of current time
long int timer2 = 0;          //timer2 interrupts counter

void setup() {
  DDRB |= 0b00000000;               //i/o initialization
  PORTB |= 0b10000000;
  DDRD |= 0b10000000;
  lcd.begin();                      //lcd initialization
  lcd.backlight();
  lcd.setCursor(4, 0);
  sprintf(_time, "%02d:%02d:%02d", h, m, s);
  lcd.print(_time);
  lcd.setCursor(cursorPos, 0);
  lcd.blink();
  cli();                            //turn off interrupts
  TCCR1A = 0;                       //timer1 interrupt initialization
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 0xF424;
  TCCR1B = (1 << WGM12) | (1 << CS12);
  TIMSK1 = (1 << OCIE1A);
  TCCR2A = 0;                       //timer2 interrupt initialization
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = 0x9C;
  TCCR2A = 1 << WGM21;
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 = (1 << OCIE2A);
  sei();                            //turn on interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (mode == 1) s -= 1;
}

ISR(TIMER2_COMPA_vect) {
  timer2 += 1;
}

void loop() {
  if (mode == 0) {                                      //setup mode
    if (timer2 > debounceDelay) {                       //button debounce handling
      if ((PINB & 0b00001) == 0) {                      //button 1 pressed
        if (cursorPos == 5) cursorPos = 11;             //move cursor left
        else cursorPos -= 3;
        pressedMode0();
      }
      if ((PINB & 0b00010) == 0) {                      //button 2 pressed
        if (cursorPos == 11) cursorPos = 5;             //move cursor right
        else cursorPos += 3;
        pressedMode0();
      }
      if ((PINB & 0b00100) == 0) {                      //button 3 pressed
        if (cursorPos == 5) {                           //decrease value under cursor
          if (h == 0) h = 23;
          else h -= 1;
        }
        else if (cursorPos == 8) {
          if (m == 0) m = 59;
          else m -= 1;
        }
        else if (cursorPos == 11) {
          if (s == 0) s = 59;
          else s -= 1;
        }
        pressedMode0();
      }
      if ((PINB & 0b01000) == 0) {                      //button 4 pressed
        if (cursorPos == 5) {                           //increase value under cursor
          if (h == 23) h = 0;
          else h += 1;
        }
        else if (cursorPos == 8) {
          if (m == 59) m = 0;
          else m += 1;
        }
        else if (cursorPos == 11) {
          if (s == 59) s = 0;
          else s += 1;
        }
        pressedMode0();
      }
      if ((PINB & 0b10000) == 0) {                      //button 5 pressed, start counting
        if (h > 0 | m > 0 | s > 0) {
          mode = 1;
          pressedMode0();
          lcd.noBlink();
          TCNT1  = 0;
        }
      }
    }
  }
  else if (mode == 1) {                                 //counting mode
    if (timer2 > (5 * debounceDelay)) {                 //button debounce handling
      if ((PINB & 0b10000) == 0) {                      //botton 5 pressed, stop counting
        cursorPos = 11;
        pressedMode0();
        lcd.blink();
        mode = 0;
      }
    }
    if (s_cpy != s) {
      s_cpy = s;
      if (s != -1 ) {}
      else if (m != 0) {
        m -= 1;
        s = 59;
      }
      else if (h != 0) {
        h -= 1;
        m = 59;
        s = 59;
      }
      pressedMode1();
    }
    if (s == 0 & m == 0 & h == 0) {                     //program done counting
      mode = 2;
      pressedMode1();
    }
  }
  else if (mode == 2) {                                 //alarm mode
    if (timer2 > debounceDelay) {
      if ((PINB & 0b10000) == 0) {                      //botton 5 pressed, stop counting
        cursorPos = 11;
        pressedMode0();
        lcd.blink();
        mode = 0;
        LED = 1;
        PORTD &= ~0b10000000;
      }
    }
    if (timer2 > blinkDelay) {                          //blinking LED
      if (LED == 0) {
        PORTD |= 0b10000000;
        LED = 1;
      }
      else {
        PORTD &= ~0b10000000;
        LED = 0;
      }
      timer2 = 0;
    }
  }
}

void pressedMode0() {        //lcd and timer handling after pressing button in mode 0
  pressedMode1();
  lcd.setCursor(cursorPos, 0);
  TCNT2  = 0;
  timer2 = 0;
}

void pressedMode1() {        //lcd handling after pressing button in mode 1
  lcd.clear();
  lcd.setCursor(4, 0);
  sprintf(_time, "%02d:%02d:%02d", h, m, s);
  lcd.print(_time);
}
