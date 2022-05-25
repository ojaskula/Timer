#include <avr/interrupt.h>
#include<avr/io.h>
#include <LiquidCrystal_I2C.h>
#define TIMSK     _SFR_IO8(0x37)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.clear();
  lcd.begin();
    lcd.backlight();
    lcd.setCursor(4,0);
    Serial.begin(9600);

  lcd.print("ixsxdfsgh");
   DDRD = 0x01;
 
  TCNT1 = 63974;   // for 1 sec at 16 MHz 

  TCCR1A = 0x00;
  TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
  TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
  sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
  
}
ISR(TIMER1_COMPA_vect)
{
lcd.print("i");
  TCNT1 = 63974;   // for 1 sec at 16 MHz
  Serial.println("i");
   
}
void loop() {
  // put your main code here, to run repeatedly:
Serial.println("i");
}
