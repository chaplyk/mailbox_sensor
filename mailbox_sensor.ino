#include <SoftwareSerial.h>

const byte HC12RxdPin = 4;                  // Recieve Pin on HC12
const byte HC12TxdPin = 5;                  // Transmit Pin on HC12

SoftwareSerial HC12(HC12TxdPin,HC12RxdPin); // Create Software Serial Port

int trigPin = 7; 
int echoArr[] = {8, 2};                     // Array with Echo Pins
int x;
  
void setup() { 
  Serial.begin (9600);
  HC12.begin(9600);                         // Open serial port to HC12
  
  pinMode(trigPin, OUTPUT);
  for (x=0; x<sizeof(echoArr)/sizeof(int); x++)
  {
    pinMode(echoArr[x], INPUT);
  }

} 
 
void loop() { 
  for (x=0; x<sizeof(echoArr)/sizeof(int); x++)
  {
    Serial.print(echoArr[x]);
    Serial.print(" PIN: ");
    Serial.println(measure(echoArr[x]));
    delay(1000);
  }
  
  Serial.print("Voltage: ");
  Serial.println(ReadVCC());
  delay(1000);

  if(Serial.available()){                   // If Arduino's computer rx buffer has data
    HC12.write(Serial.read());              // Send that data to serial
  }
}

int measure(int echoPin) {
  int duration;
  int distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  distance = duration / 58;
  return distance;
} 

long ReadVCC() {
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  uint8_t low  = ADCL;
  uint8_t high = ADCH;
  long result = (high << 8) | low;
  result = 1.1 * 1023 * 1000 / result;
  return result;
}
