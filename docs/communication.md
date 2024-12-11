## CAN Messages

### 🏎️ Speed Message

ID: 0x01

Data: Speed on byte 0, Unit on byte 1, RPM on bytes 2 and 3 (stored in little endian)

Example: 

    ID: 0x01
    Length: 4
    Data: 0x01 0x00 0x2C 0x01 0x00 0x00 0x00 0x00

    which translates to:
    - Speed: 1
    - Unit: 0x00 (km/h)
    - RPM: 300

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
