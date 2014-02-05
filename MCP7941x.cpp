/*
  MCP7941x.cpp - Arduino Library for using the MCP7941x IC.

  Ian Chilton <ian@ichilton.co.uk>
  November 2011

  Only currently supports 24hr clock and doesn't do any validation.
*/


#if (ARDUINO >= 100)
  #include <Arduino.h>
  #define WireSend(x) Wire.write(x)
  #define WireReceive() Wire.read()  
#else
  #include <WProgram.h>
  #define WireSend(x) Wire.send(x)
  #define WireReceive(x) Wire.receive(x)
#endif

#include "Wire.h"

#include "MCP7941x.h"


// Constructor:
MCP7941x::MCP7941x()
{
  Wire.begin(); 
}


// Convert normal decimal numbers to binary coded decimal:
byte MCP7941x::decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}


// Convert binary coded decimal to normal decimal numbers:
byte MCP7941x::bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}


// Function to read the mac address from the eeprom:
void MCP7941x::getMacAddress(byte *mac_address)
{ 
  Wire.beginTransmission(MCP7941x_EEPROM_I2C_ADDR);
  WireSend(MAC_LOCATION);
  Wire.endTransmission();

  Wire.requestFrom(MCP7941x_EEPROM_I2C_ADDR, 6);

  for( int i=0; i<6; i++ )
  {
    mac_address[i] = WireReceive();
  }
}


// Unlock the unique id area ready for writing:
void MCP7941x::unlockUniqueID()
{
  // Write 0x55 to the memory location 0x09 and stop:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(0x09);
  WireSend(0x55);
  Wire.endTransmission();

  // Write 0xAA to the memory location 0x09 and stop:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(0x09);
  WireSend(0xAA);
  Wire.endTransmission();
}


// Unlock the unique id area and write in the mac address:
void MCP7941x::writeMacAddress(byte *mac_address)
{
  Wire.beginTransmission(MCP7941x_EEPROM_I2C_ADDR);
  WireSend(0xF2);

  for( int i=0; i<6; i++ )
  {
    WireSend(mac_address[i]);
  }

  Wire.endTransmission();
}


// Set the date/time, set to 24hr and enable the clock:
// (assumes you're passing in valid numbers)
void MCP7941x::setDateTime(
  byte second,        // 0-59
  byte minute,        // 0-59
  byte hour,          // 1-23
  byte dayOfWeek,     // 1-7
  byte dayOfMonth,    // 1-28/29/30/31
  byte month,         // 1-12
  byte year)          // 0-99
{
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  
  WireSend(decToBcd(second) & 0x7f);              // set seconds and disable clock (01111111)
  WireSend(decToBcd(minute) & 0x7f);              // set minutes (01111111)
  WireSend(decToBcd(hour) & 0x3f);                // set hours and to 24hr clock (00111111)
  WireSend(0x08 | (decToBcd(dayOfWeek) & 0x07));  // set the day and enable battery backup (00000111)|(00001000)
  WireSend(decToBcd(dayOfMonth) & 0x3f);          // set the date in month (00111111)
  WireSend(decToBcd(month) & 0x1f);               // set the month (00011111)
  WireSend(decToBcd(year));                       // set the year (11111111)
  
  Wire.endTransmission();

  // Start Clock:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  WireSend(decToBcd(second) | 0x80);     // set seconds and enable clock (10000000)
  Wire.endTransmission();
}


// Get the date/time:
void MCP7941x::getDateTime(
  byte *second,
  byte *minute,
  byte *hour,
  byte *dayOfWeek,
  byte *dayOfMonth,
  byte *month,
  byte *year)
{
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  Wire.endTransmission();

  Wire.requestFrom(MCP7941x_RTC_I2C_ADDR, 7);
  
  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(WireReceive() & 0x7f);  // 01111111
  *minute     = bcdToDec(WireReceive() & 0x7f);  // 01111111
  *hour       = bcdToDec(WireReceive() & 0x3f);  // 00111111
  *dayOfWeek  = bcdToDec(WireReceive() & 0x07);  // 01111111
  *dayOfMonth = bcdToDec(WireReceive() & 0x3f);  // 00111111
  *month      = bcdToDec(WireReceive() & 0x1f);  // 00011111
  *year       = bcdToDec(WireReceive());         // 11111111
}


// Enable the clock without changing the date/time:
void MCP7941x::enableClock()
{
  // Get the current seconds value as the enable/disable bit is in the same
  // byte of memory as the seconds value:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  Wire.endTransmission();

  Wire.requestFrom(MCP7941x_RTC_I2C_ADDR, 1);
  
  int second = bcdToDec(WireReceive() & 0x7f);  // 01111111

  // Start Clock:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  WireSend(decToBcd(second) | 0x80);     // set seconds and enable clock (10000000)
  Wire.endTransmission();
}


// Disable the clock without changing the date/time:
void MCP7941x::disableClock()
{
  // Get the current seconds value as the enable/disable bit is in the same
  // byte of memory as the seconds value:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  Wire.endTransmission();

  Wire.requestFrom(MCP7941x_RTC_I2C_ADDR, 1);
  
  int second = bcdToDec(WireReceive() & 0x7f);  // 01111111

  // Start Clock:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  WireSend(decToBcd(second));     // set seconds and disable clock (01111111)
  Wire.endTransmission();
}



// Enable the battery:
void MCP7941x::enableBattery()
{
  // Get the current seconds value as the enable/disable bit is in the same
  // byte of memory as the seconds value:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION + 0x03);
  Wire.endTransmission();

  Wire.requestFrom(MCP7941x_RTC_I2C_ADDR, 1);
  
  int day = bcdToDec(WireReceive() & 0x07);  // 00000111

  // Start Clock:
  Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION + 0x03);
  WireSend(decToBcd(day) | 0x08);     // set day and enable battery (00001000)
  Wire.endTransmission();
}


// Store byte of data in SRAM:
void MCP7941x::setSramByte ( byte location, byte data )
{
  if (location >= 0x20 && location <= 0x5f)
  {
    Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
    WireSend(location);
    WireSend(data);
    Wire.endTransmission();
  }
}


// Read byte of data from SRAM:
byte MCP7941x::getSramByte ( byte location )
{
  if (location >= 0x20 && location <= 0x5f)
  {
    Wire.beginTransmission(MCP7941x_RTC_I2C_ADDR);
    WireSend(location);
    Wire.endTransmission();

    Wire.requestFrom(MCP7941x_RTC_I2C_ADDR, 1);

    return WireReceive();
  }
}

