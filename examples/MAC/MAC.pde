/*
   Example for the MCP7941x Library - Read MAC Address

   Ian Chilton <ian@ichilton.co.uk>
   November 2011
*/

#include "Wire.h"
#include "MCP7941x.h"

static uint8_t mymac[6] = { 0,0,0,0,0,0 };

// Create new instance of class:
MCP7941x rtc = MCP7941x();


void setup()
{
  Serial.begin(9600);

  // Get the mac address and store in mymac:
  rtc.getMacAddress(mymac);
}


void loop()
{
  // Print the mac address:
  for( int i=0; i<6; i++ )
  {
    if (mymac[i] < 10)
    {
      Serial.print(0);  
    }
    
    Serial.print( mymac[i], HEX );
    Serial.print( i < 5 ? ":" : "" );
  }

  Serial.println();
  
  // Sleep for 10s:
  delay(10000);
}

