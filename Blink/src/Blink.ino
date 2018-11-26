// Include the library:
#include "SparkFunRHT03.h"
#include "wisol_sigfox.h"
/////////////////////
// Pin Definitions //
/////////////////////
const int RHT03_DATA_PIN = D3; // RHT03 data pin
#include "SparkFunMicroOLED.h"  // Include MicroOLED library
#include "math.h"
int servo = D2;
int value = 14;
int frequency = 50;
MicroOLED oled;
///////////////////////////
// RHT03 Object Creation //
///////////////////////////
RHT03 rht; // This creates a RTH03 object, which we'll use to interact with the sensor

void setup()
{
	Serial.begin(9600); // Serial is used to print sensor readings.
	
	// Call rht.begin() to initialize the sensor and our data pin
	rht.begin(RHT03_DATA_PIN);
	Serial.begin(9600);
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.display();  // Display what's in the buffer (splashscreen)
  delay(1000);     // Delay 1000 ms
  randomSeed(analogRead(A0) + analogRead(A1));
  Particle.function("WhatIsLove",func);
  pinMode(servo, OUTPUT);
  analogWrite(servo, value , frequency);
   Serial.begin(9600);
  
  
  wisol_sigfox__serial_init();
  if (wisol_sigfox__ready())
  {
    Serial.println("Sigfox is ready");
    char buff[32];
    wisol_sigfox__get_id(buff);
    Serial.print("ID=");
    Serial.println(buff);
  }
  else 
  {
    Serial.println("module not ready");
  }
}
int func(String data)
{
  printTitle("baby",1);
  printTitle("don't\n hurt",1);
  printTitle("me",1);
  printTitle("no more",1);
  analogWrite(servo, 35 , frequency);
  delay(2000);
  analogWrite(servo, 15 , frequency);
  return 0;
}

const char *format = "%i";
char buf[16];
void loop()
{
	// Call rht.update() to get new humidity and temperature values from the sensor.
	int updateRet = rht.update();
	
	// If successful, the update() function will return 1.
	// If update fails, it will return a value <0
	if (updateRet == 1)
	{
		// The humidity(), tempC(), and tempF() functions can be called -- after 
		// a successful update() -- to get the last humidity and temperature
		// value 
		float latestHumidity = rht.humidity();
		float latestTempC = rht.tempC();
		float latestTempF = rht.tempF();
		
		// Now print the values:
		Serial.println("Humidity: " + String(latestHumidity, 1) + " %");
		Serial.println("Temp (F): " + String(latestTempF, 1) + " deg F");
		Serial.println("Temp (C): " + String(latestTempC, 1) + " deg C");
    printTitle(String(latestTempC, 0.5)+" C", 1);
    if (latestTempC > 25)
    {
      tone (D1, 700, 1000);
      Serial.println("sending message");
       int l = snprintf(buf, sizeof(buf), format, (int)latestTempC);
    if (wisol_sigfox__send_frame((const uint8_t*)buf, l , false))
    {
      Serial.println("sent.");
    }
    }
    else (noTone(D1));
    
	}
	else
	{
		// If the update failed, try delaying for RHT_READ_INTERVAL_MS ms before
		// trying again.
		delay(RHT_READ_INTERVAL_MS);
	}
	
	delay(1000);
}

void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;

  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length()/2)),
                 middleY - (oled.getFontWidth() / 2));
  // Print the title:
  oled.print(title);
  oled.display();
  delay(500);
  oled.clear(PAGE);
}
