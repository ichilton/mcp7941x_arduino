/*
   Example for the MCP7941x Library - Set/Get Date/Time

   Ian Chilton <ian@ichilton.co.uk>
   November 2011
*/

#include "Wire.h"
#include "MCP7941x.h"

// Create new instance of class:
MCP7941x rtc = MCP7941x();


void displayDateTime(
  byte second,
  byte minute,
  byte hour,
  byte dayOfWeek,
  byte dayOfMonth,
  byte month,
  byte year) 
{
  if (hour < 10)
  {
    Serial.print("0");
  }
  
  Serial.print(hour, DEC);
  Serial.print(":");

  if (minute < 10)
  {
      Serial.print("0");
  }

  Serial.print(minute, DEC);
  Serial.print(":");

  if (second < 10)
  {
    Serial.print("0");
  }

  Serial.print(second, DEC);
  Serial.print("  ");

  if (dayOfMonth < 10)
  {
    Serial.print("0");
  }
  
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");

  if (month < 10)
  {
    Serial.print("0");
  }
  
  Serial.print(month, DEC);
  Serial.print("/");
  
  Serial.print(year, DEC);

  Serial.print(" (");

  switch(dayOfWeek)
  {
    case 1: 
      Serial.print("Sunday");
      break;

    case 2: 
      Serial.print("Monday");
      break;

    case 3: 
      Serial.print("Tuesday");
      break;

    case 4: 
      Serial.print("Wednesday");
      break;

    case 5: 
      Serial.print("Thursday");
      break;

    case 6: 
      Serial.print("Friday");
      break;

    case 7: 
      Serial.print("Saturday");
      break;
  }

  Serial.println(")");  
}


void setup()
{
  Serial.begin(9600);

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

  // Set the time?
  boolean setTime = false;

  // If you have a battery, you should only need to do the set once and after that,
  // it should remember the date/time even when the power is off.

  // Set these to the values you want to set the date/time to:
  second = 0;
  minute = 59;
  hour = 23;
  dayOfWeek = 3;   // 1 = Sunday, 7 = Saturday
  dayOfMonth = 8;
  month = 11;
  year = 11;

  if (setTime == true)
  {
    // Set the Date/Time:
    rtc.setDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  }
  else
  {
    // Enable (start) the clock without changing it:
    rtc.enableClock();
  }
}


void loop()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

  // Get the date/time from the RTC:
  rtc.getDateTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  // Display the Date/Time on the serial line:
  displayDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  
  // Sleep for 1s:
  delay(1000);
}

