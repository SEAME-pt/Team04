## CAN Messages

### 🏎️ Speed Message

ID: 0x01

Data: Speed on bytes 0 through 4 (float), RPM on bytes 5 through 8 (float), both stored in little endian

Example: 

    ID: 0x01
    Length: 8
    Data: 0x40 0x90 0x00 0x00 0x43 0xad 0x00 0x00

    which translates to:
    - Speed: 4.5
    - RPM: 346

### 🔧 Settings Change Message

ID: 0x02

Data: Settings in byte 0 (currently unit as the only setting in the least significant bit)

The available settings for distance unit are: 0x00 for km/h (default) and 0x01 for mph

Example:

    ID: 0x02
    Length: 1
    Data: 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00

    which translates to:
    - Unit: 1 (mph)
