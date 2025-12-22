/* EPM7032S Parallel Programmer - ERASE V6 
PLD Altera EPM7032SLC44 in PROG MODE 
Arduino IDE 2.3.6 - Arduino MEGA
Gustavo Murta - 2024/12/20

EDIT/VPP - PG0 (PIN41) = 12V VPP control

NTPW - PL0 (PIN49) - bit value 0x01
BE   - PL1 (PIN48) - bit value 0x02
TM   - PL2 (PIN47) - bit value 0x04
MT   - PL3 (PIN46) - bit value 0x08
MTIN - PL4 (PIN45) - bit value 0x10

BEM  - PL6 (PIN43) - bit value 0x40
SBI  - PL7 (PIN42) - bit value 0x80

SS   - PC0 (PIN37) - bit value 0x01
SCK  - PC1 (PIN36) - bit value 0x02
SK   - PC2 (PIN35) - bit value 0x04

SDINA  - PC4 (PIN33) - bit value 0x10
SDOUTA - PB0 (PIN53) - bit value 0x01
SCOA   - PB1 (PIN52) - bit value 0x02

SDINB  - PC5 (PIN32) - bit value 0x20
SDOUTB - PB2 (PIN51) - bit value 0x04
SCOB   - PB3 (PIN50) - bit value 0x08

A6 - PA6 (PIN28) - bit value 0x40
A5 - PA5 (PIN27) - bit value 0x20
A4 - PA4 (PIN26) - bit value 0x10
A3 - PA3 (PIN25) - bit value 0x08
A2 - PA2 (PIN24) - bit value 0x04
A1 - PA1 (PIN23) - bit value 0x02
A0 - PA0 (PIN22) - bit value 0x01
*/

int count = 0;
uint8_t val = 0;

void setup() {
  // SAFE mode Arduino - set all PORTs as input
  DDRA = 0x00;  // configure PORTA pins as input
  DDRB = 0x00;  // configure PORTB pins as input
  DDRC = 0x00;  // configure PORTC pins as input
  DDRG = 0x00;  // configure PORTG pins as input
  DDRL = 0x00;  // configure PORTL pins as input
  
  Serial.begin(115200);  // Monitor serial 115200 bps
  Serial.println("EPM7032S Parallel Programmer - ERASE V6");
}

// EPM7032S Phase 1
void phase01EPM7032() {
  Serial.println("EPM7032S - Phase 1");
  DDRG = 0x0F;   // configure PORTG pin 0 to 3 as output
  PORTG = 0x00;  // SET pins 0 to 3 to LOW
  DDRL = 0xFF;   // configure PORTL pins as output
  PORTL = 0x00;  // SET all pins to LOW
  DDRB = 0x00;   // configure PORTB pins as input
  DDRC = 0xFF;   // configure PORTC pins as output
  PORTC = 0x00;  // SET all pins to LOW
  DDRA = 0xFF;   // configure PORTA pins as output
  PORTA = 0x00;  // SET all pins to LOW

  val = PINB;
  Serial.print("Port PB PH00: ");
  Serial.println(val, BIN);  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)

  delay(50);          // delay 50 miliseconds
  PORTL |= (1 << 0);  // set High PL0 - NTPW
  delay(50);          // delay 50 miliseconds
  PORTG |= (1 << 0);  // set High PG0 - EDIT/VPP = 12V
  delay(10);          // delay 10miliseconds
  val = PINB;
  Serial.print("Port PB PH01: ");
  Serial.println(val, BIN);  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)
  delay(200);                // delay 100 miliseconds
  PORTG &= ~(1 << 0);        // set Low PG0 - EDIT/VPP = 0V
  PORTL &= ~(1 << 0);        // set Low PL0 - NTPW
  delay(100);                // delay 100 miliseconds
}

// EPM7032 Last Phase
void lastPhaseEPM7032() {
  PORTL = 0x00;  // SET all pins to LOW
  PORTG = 0x00;  // SET pins 0 to 3 to LOW
  PORTC = 0x00;  // SET all pins to LOW
  PORTA = 0x00;  // SET all pins to LOW
  Serial.println("End");
}

void loop() {
  phase01EPM7032();
  delay(100);  // delay 100 miliseconds

  count = count + 1;
  Serial.print("Test count: ");
  Serial.println(count);
  lastPhaseEPM7032();
  Serial.println();
  delay(500);  // delay 500 miliseconds
}
