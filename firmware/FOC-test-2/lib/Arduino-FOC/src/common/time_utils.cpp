#include "time_utils.h"
#include <cstdlib>

// function buffering delay() 
// arduino uno function doesn't work well with interrupts
void _delay(unsigned long ms){
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328PB__)  || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
  // if arduino uno and other atmega328p chips
  // use while instad of delay, 
  // due to wrong measurement based on changed timer0
  unsigned long t = _micros() + ms*1000;
  while( _micros() < t ){}; 
#else
  // regular micros
  _delay(ms);
      
#endif
}


// function buffering _micros() 
// arduino function doesn't work well with interrupts
unsigned long _micros(){
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328PB__)  || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
// if arduino uno and other atmega328p chips
    //return the value based on the prescaler
    
#else
  // regular micros
  return _micros();

#endif
}
