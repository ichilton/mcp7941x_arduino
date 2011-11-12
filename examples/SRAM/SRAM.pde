/*
   Example for the MCP7941x Library - Set/Get SRAM Data

   Ian Chilton <ian@ichilton.co.uk>
   November 2011
*/

#include "Wire.h"
#include "MCP7941x.h"

// Create new instance of class:
MCP7941x rtc = MCP7941x();

void setup()
{
  Serial.begin(9600);

  // Write 64 bytes of data:
  for( int i=0; i<64; i++ )
  {
    rtc.setSramByte(0x20 + i, i);
  }

  // Read 64 bytes of data:
  for( int i=0; i<64; i++ )
  {
    Serial.print(0x20 + i, HEX);
    Serial.print(" = ");
    Serial.println(rtc.getSramByte(0x20 + i), HEX);
  }
}


void loop()
{
  // Sleep for 1s:
  delay(1000);
}

