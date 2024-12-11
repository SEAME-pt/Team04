#include <SPI.h>
#include <EEPROM.h>

// DISTANCES
#define KMPH_DISTANCE 1000.000
#define MPH_DISTANCE 1609.340
#define KMPH_SETTING 0x00
#define MPH_SETTING 0x01

// CAN MESSAGES
#define SPEED_MESSAGE_ID 0x01
#define SETTINGS_MESSAGE_ID 0x02

#define WHEEL_DIAMETER 0.067
#define CAN_2515
//#define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

// encoder pin (LM393)
int encoderPin=3;

// Speed
const int pulsesPerRevolution = 20; // Pulses per wheel revolution
const float CIRCUMFERENCE = PI * WHEEL_DIAMETER;
volatile unsigned short totalPulses = 0;
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000; //the value is a number of milliseconds, ie 1 second

// Settings
const unsigned int settingsAddress = 0; //EEPROM address to start reading from
byte distanceUnitSetting;
float distanceUnit;

void setup() {
  // Start Serial
  SERIAL_PORT_MONITOR.begin(9600);
  while(!Serial){};
  Serial.println();

  // Fetch UNIT from EEPROM
  Serial.print("Read byte from EEPROM: ");
  byte unit;
  EEPROM.get(settingsAddress, unit);
  Serial.println(unit, HEX);
  // On first run, every eeprom address will be prefilled with 0xFF
  if(unit == 0xFF) {
    setInitialSettings();
    EEPROM.get(settingsAddress, unit);
  }
  setDistanceUnit(unit);

  // Attach interrupt to CAN int pin (to receive messages)
  attachInterrupt(digitalPinToInterrupt(CAN_INT_PIN), onReceiveCanMessage, FALLING); // start receive interrupt
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
      SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
      delay(100);
  }
  SERIAL_PORT_MONITOR.println("CAN init ok!");

  // LM393
  // set the encoder pin
  pinMode(encoderPin, INPUT);
  // attach the interrupt for tracking the pulses
  // one the pulse is detected, onPulse() is called
  attachInterrupt(digitalPinToInterrupt(encoderPin), onPulse, RISING);

  startMillis = millis(); // initial start time
}

void onReceiveCanMessage() {
  SERIAL_PORT_MONITOR.println("received message");
  unsigned char len = 0;
  unsigned char buf[8];
  // iterate over all pending messages
  // If either the bus is saturated or the MCU is busy,
  // both RX buffers may be in use and reading a single
  // message does not clear the IRQ conditon.
  while (CAN_MSGAVAIL == CAN.checkReceive()) {
      // read data,  len: data length, buf: data buf
      SERIAL_PORT_MONITOR.println("checkReceive");
      CAN.readMsgBuf(&len, buf);
      unsigned long canId = CAN.getCanId();
      SERIAL_PORT_MONITOR.println(canId, HEX);

      switch(canId) {
        case SETTINGS_MESSAGE_ID:
          changeSettings(buf, len);
          break;
        default:
          SERIAL_PORT_MONITOR.println("invalid can message");
          break;
      }
  }
}

byte canMessage[4] = {0, 0, 0, 0};
void loop() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    word rpm = getRPM();
    byte speed = calculateSpeed(rpm);
    sendSpeedMessage(rpm, speed);
    totalPulses = 0; // Reset the total number of pulses
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current state.
  }
}

void onPulse(){
         // increment the total number of pulses
        totalPulses++;
}

void sendSpeedMessage(word rpm, byte speed) {
  // Assign the byte value to the first byte of the message
  canMessage[0] = speed;
  canMessage[1] = distanceUnitSetting;

  // Assign the short value to the next two bytes of the message
  canMessage[2] = lowByte(rpm);
  canMessage[3] = highByte(rpm);

  byte res = CAN.sendMsgBuf(SPEED_MESSAGE_ID, 0, 4, canMessage);

  if(CAN_OK != res) {
    SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf fail!");
    switch(res)
    {
      case CAN_FAILTX:
        SERIAL_PORT_MONITOR.println("CAN_FAILTX");
        break;
      case CAN_GETTXBFTIMEOUT:
        SERIAL_PORT_MONITOR.println("CAN_GETTXBFTIMEOUT");
        break;
      case CAN_SENDMSGTIMEOUT:
        SERIAL_PORT_MONITOR.println("CAN_SENDMSGTIMEOUT");
        break;
      default:
        SERIAL_PORT_MONITOR.println("Unknown Error");
        break;

    }
  }
  else {
    SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok!");
  }
}

/**

  RPM = Rotations (totalPulses/pulsesPerRevolution) * minute (ms) / period (ms)

*/
word getRPM() {
  return (totalPulses / (float) pulsesPerRevolution) * (60000.0 / period);
}

/**

  Speed (km/h) = angular speed * circumference * (60 / 1000)

*/
byte calculateSpeed(word rpm) {
  return round(rpm * CIRCUMFERENCE * 60.0 / distanceUnit) & 0xff;
}

void setInitialSettings() {
  Serial.println("Setting initial settings.");
  EEPROM.put(settingsAddress, KMPH_SETTING);
}

void changeSettings(unsigned char* buf, unsigned int len) {
  if(len == 0) {
    return;
  }

  SERIAL_PORT_MONITOR.println("Changing settings");
  byte unit = buf[0] & 0x01;

  if(setDistanceUnit(unit)) {
    setDistanceSetting(unit);
  }
}

void setDistanceSetting(byte setting) {
  EEPROM.put(settingsAddress, setting);
}

bool setDistanceUnit(byte unit) {
  switch(unit) {
    case KMPH_SETTING:
      SERIAL_PORT_MONITOR.println("setting to kmph");
      distanceUnitSetting = KMPH_SETTING;
      distanceUnit = KMPH_DISTANCE;
      break;
    case MPH_SETTING:
      SERIAL_PORT_MONITOR.println("setting to mph");
      distanceUnitSetting = MPH_SETTING;
      distanceUnit = MPH_DISTANCE;
      break;
    default:
      SERIAL_PORT_MONITOR.println("invalid unit");
      return false;
  }
  return true;
}

// END FILE
