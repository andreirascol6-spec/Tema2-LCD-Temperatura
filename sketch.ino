#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

volatile int ore = 18;
volatile int minute = 1;
volatile int secunde = 55;
volatile bool flagSecunda = false;

void setup() {
  lcd.begin(16, 2);
  
  noInterrupts();

  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1  = 0; 

  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  secunde++;
  if (secunde >= 60) {
    secunde = 0;
    minute++;
    if (minute >= 60) {
      minute = 0;
      ore++;
      if (ore >= 24) ore = 0;
    }
  }
  flagSecunda = true;
}

uint16_t citesteADC() {
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

void actualizeazaCeas() {
  lcd.setCursor(0, 1);
  lcd.print("Ora ");
  if (ore < 10) lcd.print("0");
  lcd.print(ore);
  lcd.print(":");
  if (minute < 10) lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  if (secunde < 10) lcd.print("0");
  lcd.print(secunde);
}

void loop() {
  if (flagSecunda) {
    flagSecunda = false;
    actualizeazaCeas();
  }
}
