#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

// --- EEPROM ADDRESS DEFINITIONS
#define LCD_BACKLIGHT_ADDRESS 1  // EEPROM address for backlight setting
#define BAUD_ADDRESS 2  // EEPROM address for Baud rate setting
#define SPLASH_SCREEN_ADDRESS 3 // EEPROM address for splash screen on/off
#define ROWS_ADDRESS 4  // EEPROM address for number of rows
#define COLUMNS_ADDRESS 5  // EEPROM address for number of columns

// --- SPECIAL COMMAND DEFINITIONS
#define BACKLIGHT_COMMAND 128  // 0x80
#define SPECIAL_COMMAND 254 // 0xFE
#define BAUD_COMMAND 129  // 0x81

// --- DATA PIN DEFINITION
#define ONE_WIRE_BUS 10

OneWire oneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
 DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.

// --- ARDUINO PIN DEFINITIONS
uint8_t RSPin = 2;
uint8_t RWPin = 3;
uint8_t ENPin = 4;
uint8_t D4Pin = 5;
uint8_t D5Pin = 6;
uint8_t D6Pin = 7;
uint8_t D7Pin = 8;
uint8_t BLPin = 9;

char inKey;  // Character received from serial input
uint8_t Cursor = 0;  // Position of cursor, 0 is top left, (rows*columns)-1 is bottom right
uint8_t LCDOnOff = 1;  // 0 if LCD is off
uint8_t blinky = 0;  // Is 1 if blinky cursor is on
uint8_t underline = 0; // Is 1 if underline cursor is on
uint8_t splashScreenEnable = 1;  // 1 means splash screen is enabled
uint8_t rows = 2;  // Number rows, will be either 2 or 4
uint8_t columns = 16; // Number of columns, will be 16 or 20
uint8_t characters; // rows * columns

// initialize the LCD at pins defined above
LiquidCrystal lcd(RSPin, RWPin, ENPin, D4Pin, D5Pin, D6Pin, D7Pin);

void setup(){
  sensors.begin();
  delay(1000);
  while(Serial.available());  // If serial data is available do this
  Serial.begin(9600); //Begin serial communication
  setBaudRate(EEPROM.read(BAUD_ADDRESS));  // initialize the serial communications:
  rows = EEPROM.read(ROWS_ADDRESS);  // Read rows and columns from EEPROM. Will default to 2x16, if not previously set
  if (rows != 4)
    rows = 2;
  columns = EEPROM.read(COLUMNS_ADDRESS);
  if (columns != 20)
    columns = 16;
  lcd.begin(columns, rows); // set up the LCD's number of rows and columns:
  pinMode(BLPin, OUTPUT);// Set up the backlight
  setBacklight(EEPROM.read(LCD_BACKLIGHT_ADDRESS));
  splashScreenEnable = EEPROM.read(SPLASH_SCREEN_ADDRESS);  // Do splashscreen if set
  if (splashScreenEnable!=0)
  {
    if (columns == 16)
    {

    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("  Temp Probe  ");
      lcd.setCursor(0, 2);
      lcd.print("   YNP Geology   ");
      delay(2000);
      lcd.clear();
    }
  }
}
void loop(){
 //Report Temperature
      sensors.requestTemperatures();
      lcd.setCursor(0, 0);
      lcd.print("Temperature C: ");
      lcd.setCursor(0, 1);
      lcd.print("     ");lcd.setCursor(0, 1);
      lcd.print(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
      delay(1000);
      lcd.clear();
}
void setBacklight(uint8_t backlightSetting){
  analogWrite(BLPin, backlightSetting);
  EEPROM.write(LCD_BACKLIGHT_ADDRESS, backlightSetting);
}
/* ----------------------------------------------------------
  setBaudRate() is called from SpecialCommands(). It receives
  a baud rate setting balue that should be between 0 and 10.
  The baud rate is then set accordingly, and the new value is
  written to EEPROM. If the EEPROM value hasn't been written
  before (255), this function will default to 9600. If the value
  is out of bounds 10<baud<255, no action is taken.
  ----------------------------------------------------------*/
void setBaudRate(uint8_t baudSetting){
  // If EEPROM is unwritten (0xFF), set it to 9600 by default
  if (baudSetting==255)
    baudSetting = 4;
   
  switch(baudSetting)
  {
    case 0:
      Serial.begin(300);
      break;
    case 1:
      Serial.begin(1200);
      break;
    case 2:
      Serial.begin(2400);
      break;
    case 3:
      Serial.begin(4800);
      break;
    case 4:
      Serial.begin(9600);
      break;
    case 5:
      Serial.begin(14400);
      break;
    case 6:
      Serial.begin(19200);
      break;
    case 7:
      Serial.begin(28800);
      break;
    case 8:
      Serial.begin(38400);
      break;
    case 9:
      Serial.begin(57600);
      break;
    case 10:
      Serial.begin(115200);
      break;
  }
  if ((baudSetting>=0)&&(baudSetting<=10))
    EEPROM.write(BAUD_ADDRESS, baudSetting);
}
