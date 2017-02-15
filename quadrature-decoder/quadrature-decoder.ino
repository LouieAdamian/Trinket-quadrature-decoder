#include "SendOnlySoftwareSerial.h"
#include "avr/interrupt.h"

SendOnlySoftwareSerial mySerial(1); // RX, TX
volatile int value = 122;
volatile int lastEncoded = 0;

void setup()
{
  mySerial.begin(9600);
  pinMode(0, OUTPUT);  // set LED pin to output

  // set pins 3 and 4 to input
  // and enable pullup resisters
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);

  GIMSK = 0b00100000;       // Enable pin change interrupts
  PCMSK = 0b00011000;       // Enable pin change interrupt for PB3 and PB4
  sei();                    // Turn on interrupts
}

void loop()
{

  mySerial.println(value);
}

// This is the ISR that is called on each interrupt
// Taken from http://bildr.org/2012/08/rotary-encoder-arduino/
ISR(PCINT0_vect)
{
  int MSB = digitalRead(3); //MSB = most significant bit
  int LSB = digitalRead(4); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    value++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    value--;

  lastEncoded = encoded; //store this value for next time

  if (value <= 0)
    value = 0;
  if (value >= 255)
    value = 255;
}

