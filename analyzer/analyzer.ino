#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//
// Hardware configuration
//
#define CSN D8
#define CE D2
// Set up nRF24L01 radio on SPI bus plus pins CE & CSN

RF24 radio(CE,CSN);

//
// Define Channel Addresses
//
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.

//
// Define Constants
//
const uint8_t num_channels = 126;
const int num_reps = 100;

static const char rf24_datarate_e_str_0[] PROGMEM = "1MBPS";
static const char rf24_datarate_e_str_1[] PROGMEM = "2MBPS";
static const char rf24_datarate_e_str_2[] PROGMEM = "250KBPS";
static const char * const rf24_datarate_e_str_P[] PROGMEM = {
  rf24_datarate_e_str_0,
  rf24_datarate_e_str_1,
  rf24_datarate_e_str_2,
};

static const char rf24_model_e_str_0[] PROGMEM = "nRF24L01";
static const char rf24_model_e_str_1[] PROGMEM = "nRF24L01+";
static const char * const rf24_model_e_str_P[] PROGMEM = {
  rf24_model_e_str_0,
  rf24_model_e_str_1,
};

static const char rf24_crclength_e_str_0[] PROGMEM = "Disabled";
static const char rf24_crclength_e_str_1[] PROGMEM = "8 bits";
static const char rf24_crclength_e_str_2[] PROGMEM = "16 bits" ;
static const char * const rf24_crclength_e_str_P[] PROGMEM = {
  rf24_crclength_e_str_0,
  rf24_crclength_e_str_1,
  rf24_crclength_e_str_2,
};

static const char rf24_pa_dbm_e_str_0[] PROGMEM = "PA_MIN";
static const char rf24_pa_dbm_e_str_1[] PROGMEM = "PA_LOW";
static const char rf24_pa_dbm_e_str_2[] PROGMEM = "PA_HIGH";
static const char rf24_pa_dbm_e_str_3[] PROGMEM = "PA_MAX";
static const char * const rf24_pa_dbm_e_str_P[] PROGMEM = {
  rf24_pa_dbm_e_str_0,
  rf24_pa_dbm_e_str_1,
  rf24_pa_dbm_e_str_2,
  rf24_pa_dbm_e_str_3,
};

//
// Define Variables
//
uint8_t values[num_channels];
uint8_t zeroVal = 0xf;
uint8_t addr_width = 5;
bool p_variant = false;

//
// Setup
//

void setup(void)
{
  // Start Serial port
  Serial.begin(115200);
  delay(200);
  Serial.println();Serial.println();

  // Setup and configure rf radio
  radio.begin();
  radio.setAutoAck(false);
  //radio.openWritingPipe(pipes[0]);
  //radio.openReadingPipe(1,pipes[1]);
  
  // Get into standby mode
  radio.startListening();
  delayMicroseconds(130);
  radio.stopListening(); 

  // Display Radio Configuration
  getradiodetails();
  radio.setDataRate(RF24_1MBPS);
  
  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    Serial.print(i>>4);
    ++i;
  }
  Serial.println();
  i = 0;
  while ( i < num_channels )
  {
    Serial.print(i&0xf, HEX);
    ++i;
  }
  Serial.println();
}

//
// Main Loop
//

void loop()
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
      delayMicroseconds(130);
      radio.stopListening();

      // Did we get a carrier?
      if ( radio.testCarrier() ){
        ++values[i];
      }
    }
    yield();
  }

  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    Serial.print(min(zeroVal,values[i]));
    ++i;
  }
  Serial.println();
  yield();
}

///////////////////////////////////////////////////////////////////////////////////////
///  Define FUNCTIONS
//   These functions have been extracted from the RF24 library
//   and modified to work on the ESP8266 under the Arduino framework
//   Feel free to make corrections or efficiency improvements as you see
//   fit.

void getradiodetails()
{
  Serial.println();Serial.println();
  //print_status(spiTrans(NOP));
  print_address_register("RX_ADDR_P0-1\t",RX_ADDR_P0,2);
  print_byte_register("RX_ADDR_P2-5\t",RX_ADDR_P2,4);
  print_address_register("TX_ADDR\t\t",TX_ADDR, 1);
  print_byte_register("RX_PW_P0-6\t\t",RX_PW_P0,6);
  print_byte_register("EN_AA\t\t",EN_AA,1);
  print_byte_register("EN_RXADDR\t\t",EN_RXADDR,1);
  print_byte_register("RF_CH\t\t",RF_CH,1);
  print_byte_register("RF_SETUP\t\t",RF_SETUP,1);
  print_byte_register("CONFIG\t\t",NRF_CONFIG,1);
  print_byte_register("DYNPD/FEATURE\t",DYNPD,2);
  String statTemp = rf24_datarate_e_str_P[getDataRate()];
  Serial.printf("Data Rate\t\t= %s\r\n", statTemp.c_str());
  statTemp = rf24_model_e_str_P[isPVariant()];
  Serial.printf("Model\t\t= %s\r\n", statTemp.c_str());
  statTemp = rf24_crclength_e_str_P[getCRCLength()];
  Serial.printf("CRC Length\t\t= %s\r\n", statTemp.c_str());
  statTemp = rf24_pa_dbm_e_str_P[getPALevel()];
  Serial.printf("PA Power\t\t= %s\r\n", statTemp.c_str());
  Serial.println();Serial.println();
}

uint8_t spiTrans(uint8_t cmd)
{
  uint8_t status;
  beginTransaction();
  status = SPI.transfer( cmd );
  endTransaction();
  return status;
}

void print_status(uint8_t status)
{
  printf_P(PSTR("STATUS\t\t= 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\r\n"),
           status,
           (status & _BV(RX_DR))?1:0,
           (status & _BV(TX_DS))?1:0,
           (status & _BV(MAX_RT))?1:0,
           ((status >> RX_P_NO) & 0x07),
           (status & _BV(TX_FULL))?1:0
          );
}

void beginTransaction()
{
  digitalWrite(CSN, LOW);
}

void endTransaction()
{
  digitalWrite(CSN, HIGH);
}

void print_address_register(const char* name, uint8_t reg, uint8_t qty)
{
  char tempBuff[20];
  String valueBuf;
  valueBuf += name;
  valueBuf += "=";
  while (qty--)
  {
    uint8_t buffer[addr_width];
    read_register(reg++,buffer,sizeof buffer);
    valueBuf += " 0x";
    uint8_t* bufptr = buffer + sizeof buffer;
    while( --bufptr >= buffer ){
      sprintf(tempBuff, "%02x" ,*bufptr);
      valueBuf += tempBuff;
    }
  }
   valueBuf +="\r\n";
   Serial.print(valueBuf);
}

uint8_t read_register(uint8_t reg, uint8_t* buf, uint8_t len)
{
  uint8_t status;
  beginTransaction();
  status = SPI.transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
  while ( len-- ){
    *buf++ = SPI.transfer(0xff);
  }
  endTransaction();
  return status;
}

void print_byte_register(const char* name, uint8_t reg, uint8_t qty)
{
  char tempBuff[20];
  String valueBuf;
  valueBuf += name;
  valueBuf += "=";
  while (qty--){
    sprintf(tempBuff ," 0x%02x" , read_register(reg++));
    valueBuf += tempBuff;
  }
  valueBuf += "\r\n";
  Serial.print(valueBuf);
}

uint8_t read_register(uint8_t reg)
{
  uint8_t result;
  beginTransaction();
  SPI.transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
  result = SPI.transfer(0xff);
  endTransaction();
  return result;
}

rf24_datarate_e getDataRate( void )
{
  rf24_datarate_e result ;
  uint8_t dr = read_register(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

  // switch uses RAM (evil!)
  // Order matters in our case below
  if ( dr == _BV(RF_DR_LOW) )
  {
    // '10' = 250KBPS
    result = RF24_250KBPS ;
  }
  else if ( dr == _BV(RF_DR_HIGH) )
  {
    // '01' = 2MBPS
    result = RF24_2MBPS ;
  }
  else
  {
    // '00' = 1MBPS
    result = RF24_1MBPS ;
  }
  return result ;
}

bool isPVariant(void)
{
  p_variant = false ;
  if( radio.setDataRate( RF24_250KBPS ) )
  {
    p_variant = true ;
  }
  byte regRead = read_register(RF_SETUP);
  if( regRead == 0b00001110 )     // register default for nRF24L01P
  {
    p_variant = true ;
  }
  return p_variant ;
}

rf24_crclength_e getCRCLength(void)
{
  rf24_crclength_e result = RF24_CRC_DISABLED;
  
  uint8_t config = read_register(NRF_CONFIG) & ( _BV(CRCO) | _BV(EN_CRC)) ;
  uint8_t AA = read_register(EN_AA);
  
  if ( config & _BV(EN_CRC ) || AA)
  {
    if ( config & _BV(CRCO) )
      result = RF24_CRC_16;
    else
      result = RF24_CRC_8;
  }

  return result;
}

uint8_t getPALevel(void)
{

  return (read_register(RF_SETUP) & (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH))) >> 1 ;
}
