// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13
#include <SPI.h>

// DISTANCES
#define KMPH_SETTING 0x00
#define MPH_SETTING 0x01

#define CAN_2515
// #define CAN_2518FD

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

void setup() {
    SERIAL_PORT_MONITOR.begin(9600);

    while (CAN_OK != CAN.begin(CAN_500KBPS, MCP_8MHz)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

void loop() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        SERIAL_PORT_MONITOR.println("-----------------------------");
        SERIAL_PORT_MONITOR.print("Get data from ID: 0x");
        SERIAL_PORT_MONITOR.println(canId, HEX);

        // SPEED
        byte speed = buf[0];
        byte distanceUnit = buf[1];

        // RPM
        byte lowByte = buf[2];
        byte highByte = buf[3];
        word rpm = word(highByte, lowByte);

        SERIAL_PORT_MONITOR.print("SPEED: ");
        SERIAL_PORT_MONITOR.print(speed);
        SERIAL_PORT_MONITOR.print("\t");
        SERIAL_PORT_MONITOR.print(distanceUnit == KMPH_SETTING ? "KMPH" : "MPH");
        SERIAL_PORT_MONITOR.print("\t");
        SERIAL_PORT_MONITOR.print("RPM: ");
        SERIAL_PORT_MONITOR.print(rpm);
        SERIAL_PORT_MONITOR.println();
    }
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
