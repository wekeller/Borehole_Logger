//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
SoftwareSerial lcd(2, 3);

void setup(void)
{
  Serial.begin(9600); //Begin serial communication
  lcd.begin(9600);  // start lcd serial
  Serial.println("Arduino Digital Temperature // Serial Monitor Version"); //Print a message
  sensors.begin();
  while(Serial.available());  // If serial data is available do this
}

void loop(void)
{ 
  // Send the command to get temperatures
  sensors.requestTemperatures();  
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  //Update value every 1 sec.
  lcd.print(sensors.getTempCByIndex(0));
  delay(1000);
  clearDisplay();
  delay(50);
}

void clearDisplay()
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}
