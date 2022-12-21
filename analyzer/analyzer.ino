#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "printf.h"

#define CE  D7

#define CHANNELS  64
int channel[CHANNELS];

int  line;
char grey[] = " .:-=+*aRW";

#define _NRF24_CONFIG      0x00
#define _NRF24_EN_AA       0x01
#define _NRF24_RF_CH       0x05
#define _NRF24_RF_SETUP    0x06
#define _NRF24_RPD         0x09

RF24 radio(8, 2);

byte count;
byte sensorArray[128];


byte getRegister(byte r)
{
  byte c;

  // PORTB &= ~_BV(2);
  c = SPI.transfer(r & 0x1F);
  c = SPI.transfer(0);
  // PORTB |= _BV(2);

  return (c);
}

void setRegister(byte r, byte v)
{
  // PORTB &= ~_BV(2);
  SPI.transfer((r & 0x1F) | 0x20);
  SPI.transfer(v);
  // PORTB |= _BV(2);
}

void powerUp(void)
{
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x02);
  delayMicroseconds(130);
}

void powerDown(void)
{
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) & ~0x02);
}

//void enable(void)
//{
//  PORTB |= _BV(1);
//}
//
//void disable(void)
//{
//  PORTB &= ~_BV(1);
//}

void setRX(void)
{
  setRegister(_NRF24_CONFIG, getRegister(_NRF24_CONFIG) | 0x01);
  // enable();

  delayMicroseconds(100);
}

void scanChannels(void)
{
  // disable();
  for ( int j = 0 ; j < 200  ; j++)
  {
    for ( int i = 0 ; i < CHANNELS ; i++)
    {
      setRegister(_NRF24_RF_CH, (128 * i) / CHANNELS);

      setRX();

      delayMicroseconds(40);

      // disable();

      if ( getRegister(_NRF24_RPD) > 0 )   channel[i]++;
    }
  }
}

void outputChannels(void)
{
  int norm = 0;

  for ( int i = 0 ; i < CHANNELS ; i++)
    if ( channel[i] > norm ) norm = channel[i];

  Serial.print('|');
  for ( int i = 0 ; i < CHANNELS ; i++)
  {
    int pos;

    if ( norm != 0 ) pos = (channel[i] * 10) / norm;
    else          pos = 0;

    if ( pos == 0 && channel[i] > 0 ) pos++;

    if ( pos > 9 ) pos = 9;

    Serial.print(grey[pos]);
    channel[i] = 0;
  }

  Serial.print("| ");
  Serial.println(norm);

  for (count = 80; count >= 2; count--)
  {
    sensorArray[count - 1] = sensorArray[count - 2];
  }
}

void printChannels(void)
{
  Serial.println(">      1 2  3 4  5  6 7 8  9 10 11 12 13  14                     <");
}

// the setup function runs once when you press reset or power the board

// CE = 7, CSN = 8, SCK = 5, M0 (MOSI) = D7, M1 (MISO) = D6,
void setup() {
  for (count = 0; count <= 128; count++)
  {
    sensorArray[count] = 0;
  }

  Serial.begin(115200);
  Serial.println("Starting 2.4GHz Scanner ...");
  Serial.println();


  Serial.println("Channel Layout");
  printChannels();

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);

  pinMode(CE, OUTPUT);
  // disable();

  powerUp();

  setRegister(_NRF24_EN_AA, 0x0);


  setRegister(_NRF24_RF_SETUP, 0x0F);
}

void loop()
{

  scanChannels();

  outputChannels();

  if ( line++ > 12 )
  {
    printChannels();
    line = 0;
  }
}
