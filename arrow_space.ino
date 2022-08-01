#include "UsbKeyboard.h"

class Button{
  private:
    byte key;
    int lastRead;
    byte pin;
  public:
    Button(byte pin, byte key){
      this->pin = pin;
      this->key = key;
      this->lastRead = 1;

      init();
    }

    void init(){    
      pinMode(pin, INPUT);
      digitalWrite(pin, HIGH);
    }

    void check(){
      int rd = digitalRead(pin);
        if (rd == 0 && lastRead != 0) {

          UsbKeyboard.sendKeyStroke(key);
        
          #if BYPASS_TIMER_ISR  // check if timer isr fixed.
              delayMs(20);
          #else
              delay(20);
          #endif
        }
        lastRead = rd;
    }
};

// If the timer isr is corrected
// to not take so long change this to 0.
#define BYPASS_TIMER_ISR 1


  
  Button btn[5] = {
    Button(12, KEY_SPACE),
    Button(13, KEY_ARROW_LEFT),
    Button(10, KEY_ARROW_RIGHT),
    Button(7, KEY_ARROW_DOWN),
    Button(8, KEY_ARROW_UP),
  };
  
void setup() {
  
#if BYPASS_TIMER_ISR
  // disable timer 0 overflow interrupt (used for millis)
  TIMSK0&=!(1<<TOIE0); // ++
#endif
}

#if BYPASS_TIMER_ISR
void delayMs(unsigned int ms) {
   /*
  */ 
  for (int i = 0; i < ms; i++) {
    delayMicroseconds(1000);
  }
}
#endif

void loop() {
  
  UsbKeyboard.update();


  for(int i = 0; i < sizeof(btn)/4;i++){
    btn[i].check();
  }
}
