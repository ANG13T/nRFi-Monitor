#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "printf.h"

const uint8_t num_channels = 128;
uint8_t values[num_channels];

#define CE_PIN  D2
#define CSN_PIN D8
 
RF24 radio(CE_PIN,CSN_PIN);
 
//
// Setup
//
 
void setup(void)
{
  //
  // Print preamble
  //
 
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/examples/scanner/\n\r");
 
  //
  // Setup and configure rf radio
  //
 
  radio.begin();
  radio.setAutoAck(false);
 
  // Get into standby mode
  radio.startListening();
  radio.stopListening();
 
  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x",i>>4);
    ++i;
  }
  printf("\n\r");
  i = 0;
  while ( i < num_channels )
  {
    printf("%x",i&0xf);
    ++i;
  }
  printf("\n\r");
}
 
//
// Loop
//
 
const int num_reps = 100;
 
void loop(void)
{
  // Clear measurement values
  memset(values,0,sizeof(values));
 
  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--)
  {
    int i = num_channels;
    while (i--)
    {
      // Select this channel
      radio.setChannel(i);
 
      // Listen for a little
      radio.startListening();
      delayMicroseconds(225);
      
 
      // Did we get a carrier?
      if ( radio.testCarrier() ){
        ++values[i];
      }
      radio.stopListening();
    }
  }
 
  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x",min(0xf,values[i]&0xf));
    ++i;
  }
  printf("\n\r");
}
 
