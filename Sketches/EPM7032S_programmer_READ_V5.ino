/* EPM7032S Parallel Programmer - READ Version 5 
PLD Altera EPM7032SLC44 in PROG MODE 
Arduino IDE 2.3.6 - Arduino MEGA
Gustavo Murta - 2026_01_13

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
int pulses = 0;
uint8_t bufferSCOX[16];

uint8_t byteSCOA = 0;
uint8_t byteSCOA1 = 0;
uint8_t byteSCOA2 = 0;
uint8_t byteSCOB = 0;
uint8_t byteSCOB1 = 0;
uint8_t byteSCOB2 = 0;

void setup() {
  // SAFE mode Arduino - set all PORTs as input
  DDRA = 0x00;  // configure PORTA pins as input
  DDRB = 0x00;  // configure PORTB pins as input
  DDRC = 0x00;  // configure PORTC pins as input
  DDRG = 0x00;  // configure PORTG pins as input
  DDRL = 0x00;  // configure PORTL pins as input

  Serial.begin(115200);  // Monitor serial 115200 bps
  Serial.println("EPM7032S Parallel Programmer - READ V5");
  delay(2000);  // delay 2 seconds
}

// EPM7032S Phase 1 - NTPW High and Edit/VPP High
void phase01EPM7032() {
  DDRA = 0xFF;   // configure PORTA pins as output
  PORTA = 0x00;  // SET all pins to LOW
  DDRG = 0x0F;   // configure PORTG pin 0 to 3 as output
  PORTG = 0x00;  // SET pins 0 to 3 to LOW
  DDRL = 0xFF;   // configure PORTL pins as output
  PORTL = 0x00;  // SET all pins to LOW
  DDRB = 0x00;   // configure PORTB pins as input
  DDRC = 0xFF;   // configure PORTC pins as output
  PORTC = 0x00;  // SET all pins to LOW
  Serial.println("EPM7032S - Phase 1");

  //val = PINB;
  //Serial.print("Port PB PH00: ");  // only for tests
  //Serial.println(val, BIN);  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)

  PORTL |= (1 << 0);      // set High NTPW (PL0)
  delay(50);              // delay 50 miliseconds
  PORTG |= (1 << 0);      // set High EDIT/VPP = 12V (PG0)
  delayMicroseconds(50);  // delay 50 microseconds
  //val = PINB;
  //Serial.print("Port PB PH01: ");  // only for tests
  //Serial.println(val, BIN);  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)
  //delayMicroseconds(50);  // delay 50 microseconds
}

// Send SDINA and SDINB pulses to CPLD
void pulses_SDIN() {
  for (int i = 0; i < 16; i++) {   // loop 16 times
    for (int c = 0; c < 4; c++) {  // loop 4 times
      PORTC |= (1 << 1);           // set High SCK (PC1)
      delayMicroseconds(6);        // delay 5 microseconds
      PORTC &= ~(1 << 1);          // set Low SCK (PC1)
      delayMicroseconds(11);       // delay 10 microseconds
    }
    PORTC |= (1 << 4);     // set High SDINA (PC4)
    delayMicroseconds(6);  // delay 5 microseconds
    PORTC |= (1 << 5);     // set High SDINB (PC5)
    delayMicroseconds(6);  // delay 5 microseconds
    PORTC |= (1 << 1);     // set High SCK (PC1)
    delayMicroseconds(6);  // delay 5 microseconds
    PORTC &= ~(1 << 1);    // set Low SCK (PC1)
    delayMicroseconds(6);  // delay 5 microseconds
    PORTC &= ~(1 << 4);    // set Low SDINA (PC4)
    delayMicroseconds(6);  // delay 5 microseconds
    PORTC &= ~(1 << 5);    // set Low SDINB (PC5)
    delayMicroseconds(6);  // delay 5 microseconds
  }
}

// Set Address 0x40 (A6=1) loop X times
void pulses_Address40(uint8_t pulses) {
  // Serial.println("Pulses Address 0x40");
  for (int i = 0; i < pulses; i++) {  // loop X times
    PORTA = 0x40;                     // set High A6 (PA6)
    delayMicroseconds(6);             // delay 5 microseconds
    PORTA = 0x00;                     // set Low A6 (PA6)
    delayMicroseconds(6);             // delay 5 microseconds
  }
}

// Set Address 0x43 (A6=1 A1=1 A0=1 )
void pulses_Address43() {
  // Serial.println("Pulses Address 0x43");
  // delayMicroseconds(6);  // delay 5 microseconds
  PORTA = 0x43;          // set Address = 0x43
  delayMicroseconds(6);  // delay 5 microseconds
  PORTA = 0x00;          // set Address = 0x00
  delayMicroseconds(6);  // delay 5 microseconds
}

// Set Address 0x5B (A6=1 A4=1 A3=1 A1=1 A0=1 )
void pulse_Address5B() {
  // Serial.println("Pulse Address 0x5B");
  // delayMicroseconds(6);  // delay 5 microseconds
  PORTA = 0x5B;          // set Address = 0x5B
  delayMicroseconds(6);  // delay 5 microseconds
  PORTA = 0x00;          // set Address = 0x00
  delayMicroseconds(6);  // delay 5 microseconds
}

// Print Bytes from SCOA and SCOB pins
void printSCOX() {
  Serial.print(" ByteSCOA = ");
  Serial.print(byteSCOA2, HEX);  // HEX or (char)
  Serial.print("(");
  Serial.print((char)byteSCOA2);  // HEX or (char)
  Serial.print(")  ");
  Serial.print(byteSCOA1, HEX);  // HEX or (char)
  Serial.print("(");
  Serial.print((char)byteSCOA1);  // HEX or (char)
  Serial.print(") ");

  Serial.print("     ByteSCOB = ");
  Serial.print(byteSCOB2, HEX);  // HEX or (char)
  Serial.print("(");
  Serial.print((char)byteSCOB2);  // HEX or (char)
  Serial.print(") ");
  Serial.print(byteSCOB1, HEX);  // HEX or (char)
  Serial.print("(");
  Serial.print((char)byteSCOB1);  // HEX or (char)
  Serial.print(") ");
}

// Read Bytes from SCOA e SCOB pins
void readSCOX() {
  // Serial.println("Read SCOX");
  delayMicroseconds(6);  // delay 5 microseconds

  for (int c = 0; c < 5; c++) {  // loop 5 times

    PORTC |= (1 << 2);      // set High SK (PC2)
    delayMicroseconds(6);   // delay 5 microseconds
    PORTC &= ~(1 << 2);     // set Low SK (PC2)
    delayMicroseconds(10);  // delay 10 microseconds
    PORTC |= (1 << 0);      // set High SS (PC0)

    for (int b = 0; b < 2; b++) {            // loop 2 times
      for (int i = 0; i < 8; i++) {          // loop 8 times
        delayMicroseconds(150);              // delay 150 microseconds
        PORTC |= (1 << 2);                   // set High SK (PC2)
        val = PINB & ((1 << 1) | (1 << 3));  // read SCOA(PB1) and SCOB(PB3)
        bufferSCOX[i] = val;                 // input bytes to Buffer SCOX
        delayMicroseconds(6);                // delay 5 microseconds
        PORTC &= ~(1 << 2);                  // set Low SK (PC2)
        if (i == 0) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 1;  // (x & (1 << 3))
          if (val & (1 << 3)) byteSCOB = byteSCOB + 1;
        }
        if (i == 1) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 2;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 2;
        }
        if (i == 2) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 4;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 4;
        }
        if (i == 3) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 8;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 8;
        }
        if (i == 4) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 16;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 16;
        }
        if (i == 5) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 32;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 32;
        }
        if (i == 6) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 64;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 64;
        }
        if (i == 7) {
          if (val & (1 << 1)) byteSCOA = byteSCOA + 128;
          if (val & (1 << 3)) byteSCOB = byteSCOB + 128;
        }
      }
      if (b == 0) {
        byteSCOA1 = byteSCOA;
        byteSCOB1 = byteSCOB;
      }
      if (b == 1) {
        byteSCOA2 = byteSCOA;
        byteSCOB2 = byteSCOB;
      }
      byteSCOA = 0;
      byteSCOB = 0;
    }

    delayMicroseconds(50);  // delay 50 microseconds
    PORTC &= ~(1 << 0);     // set Low SS (PC0)
    printSCOX();            // Print Bytes SCOA and SCOB
    delayMicroseconds(50);  // delay 50 microseconds

    if (c != 4) {
      PORTC |= (1 << 1);       // set High SCK (PC1)
      delayMicroseconds(6);    // delay 5 microseconds
      PORTC &= ~(1 << 1);      // set Low SCK (PC1)
      delayMicroseconds(200);  // delay 200 microseconds
    }
    Serial.println();
  }
}

// EPM7032 Last Phase - set all pins to LOW
void lastPhaseEPM7032() {
  Serial.println("Last Phase");
  PORTG &= ~(1 << 0);     // set Low EDIT/VPP (PG0)
  delayMicroseconds(50);  // delay 50 microseconds
  PORTL &= ~(1 << 0);     // set Low NPW (PL0)
  delayMicroseconds(50);  // delay 50 microseconds
  PORTL = 0x00;           // SET all pins to LOW
  PORTC = 0x00;           // SET all pins to LOW
  PORTA = 0x00;           // SET all pins to LOW
  Serial.println("End");
  Serial.println();
}

void loop() {

  count = count + 1;
  Serial.print("Test count: ");
  Serial.println(count);

  phase01EPM7032();
  pulses_SDIN();          // Send SDINA and SDINB pulses to CPLD
  delayMicroseconds(20);  // delay 20 microseconds
  pulses_Address40(214);  // 214 pulses of Address 0x40
  delayMicroseconds(6);   // delay 5 microseconds
  pulses_Address43();     // Set Address 0x43
  delayMicroseconds(6);   // delay 5 microseconds
  Serial.println("Read IDCODE");
  readSCOX();
  delayMicroseconds(100);  // delay 100 microseconds

  pulses_SDIN();          // Send SDINA and SDINB pulses to CPLD
  delayMicroseconds(20);  // delay 20 microseconds
  pulses_Address40(213);  // 213 pulses of Address 0x40
  pulse_Address5B();      // set Address 0x5B
  pulses_Address40(1);    // 1 pulse of Address 0x40
  Serial.println();
  Serial.println("Read SCOA and SCOB");
  readSCOX();
  delayMicroseconds(100);  // delay 100 microseconds

  lastPhaseEPM7032();
  delay(700);  // delay 500 miliseconds
}
