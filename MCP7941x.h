/*
  MCP7941x.h - Arduino Library for using the MCP7941x IC.

  Ian Chilton <ian@ichilton.co.uk>
  November 2011
*/

#ifndef MCP7941x_h
#define MCP7941x_h

  #define MCP7941x_EEPROM_I2C_ADDR 0x57
  #define MAC_LOCATION 0xF2   // Starts at 0xF0 but we are only interested in 6 bytes.

  #define MCP7941x_RTC_I2C_ADDR 0x6F
  #define RTC_LOCATION 0x00


  #if (ARDUINO >= 100)
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif


  class MCP7941x
  {
    public:

      MCP7941x();

      byte decToBcd ( byte val );
      byte bcdToDec ( byte val );

      void getMacAddress ( byte *mac_address );
      void unlockUniqueID();
      void writeMacAddress ( byte *mac_address );

      void setDateTime ( byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year );
      void getDateTime ( byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year );
      void enableClock ();
      void disableClock ();
      void enableBattery ();

      void setSramByte ( byte location, byte data );
      byte getSramByte ( byte location );

    private:

  };

#endif
