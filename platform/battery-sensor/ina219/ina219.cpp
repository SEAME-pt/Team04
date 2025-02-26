#include "ina219.hpp"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cmath>
#include <cstdio>

INA219::INA219(float shunt_resistance, float max_expected_amps) {
    initI2c(__ADDRESS);

    _shunt_ohms = shunt_resistance;
    _max_expected_amps = max_expected_amps;
    _min_device_current_lsb =
        static_cast<float>(__CALIBRATION_FACTOR) / (_shunt_ohms * __MAX_CALIBRATION_VALUE);
}
INA219::INA219(float shunt_resistance, float max_expected_amps, uint8_t address) {
    initI2c(address);

    _shunt_ohms = shunt_resistance;
    _max_expected_amps = max_expected_amps;
    _min_device_current_lsb =
        static_cast<float>(__CALIBRATION_FACTOR) / (_shunt_ohms * __MAX_CALIBRATION_VALUE);
}
INA219::~INA219() { close(_file_descriptor); }

void INA219::initI2c(uint8_t address) {
    const char *filename = "/dev/i2c-1";
    _file_descriptor = open(filename, O_RDWR);
    if (_file_descriptor < 0) {
        perror("Failed to open the i2c bus");
    }
    if (ioctl(_file_descriptor, I2C_SLAVE, address) < 0) {
        perror("Failed to acquire bus access and/or talk to slave device: ");
    }
}

auto INA219::readRegister(uint8_t register_value) -> uint16_t {
    uint8_t buf[3];
    buf[0] = register_value;
    if (write(_file_descriptor, buf, 1) != 1) {
        perror("Failed to set register");
    }
    usleep(1000);
    if (read(_file_descriptor, buf, 2) != 2) {
        perror("Failed to read register value");
    }
    return (buf[0] << 8) | buf[1];
}
void INA219::writeRegister(uint8_t register_address, uint16_t register_value) {
    uint8_t buf[3];
    buf[0] = register_address;
    buf[1] = static_cast<uint8_t>(register_value >> 8);
    buf[2] = static_cast<uint8_t>(register_value & 0xFF);

    if (write(_file_descriptor, buf, 3) != 3) {
        perror("Failed to write to the i2c bus");
    }
}

void INA219::configure(int voltage_range, int gain, int bus_adc, int shunt_adc) {
    reset();

    int len = sizeof(__BUS_RANGE) / sizeof(__BUS_RANGE[0]);
    if (voltage_range > len - 1) {
        perror("Invalid voltage range, must be one of: RANGE_16V, RANGE_32");
    }
    _voltage_range = voltage_range;
    _gain = gain;

    calibrate(__BUS_RANGE[voltage_range], __GAIN_VOLTS[gain], _max_expected_amps);
    auto calibration =
        static_cast<uint16_t>((voltage_range << __BRNG | _gain << __PG0 | bus_adc << __BADC1 |
                               shunt_adc << __SADC1 | __CONT_SH_BUS));
    writeRegister(__REG_CONFIG, calibration);
}

void INA219::calibrate(int bus_volts_max, float shunt_volts_max, float max_expected_amps) {
    (void)bus_volts_max;
    float max_possible_amps = shunt_volts_max / _shunt_ohms;
    _current_lsb = determineCurrentLsb(max_expected_amps, max_possible_amps);
    _power_lsb = _current_lsb * 20.0F;
    auto calibration =
        static_cast<uint16_t>(trunc(__CALIBRATION_FACTOR / (_current_lsb * _shunt_ohms)));
    writeRegister(__REG_CALIBRATION, calibration);
}
auto INA219::determineCurrentLsb(float max_expected_amps, float max_possible_amps) -> float {
    float current_lsb;

    float nearest = roundf(max_possible_amps * 1000.0F) / 1000.0F;
    if (max_expected_amps > nearest) {
        char buffer[80];
        sprintf(buffer, "Expected current %f A is greater than max possible current %f A",
                max_expected_amps, max_possible_amps);
        perror(buffer);
    }

    if (max_expected_amps < max_possible_amps) {
        current_lsb = max_expected_amps / __CURRENT_LSB_FACTOR;
    } else {
        current_lsb = max_possible_amps / __CURRENT_LSB_FACTOR;
    }

    if (current_lsb < _min_device_current_lsb) {
        current_lsb = _min_device_current_lsb;
    }
    return current_lsb;
}
void INA219::sleep() {
    uint16_t config = readRegister(__REG_CONFIG);
    writeRegister(__REG_CONFIG, config & 0xFFF8);
}
void INA219::wake() {
    uint16_t config = readRegister(__REG_CONFIG);
    writeRegister(__REG_CONFIG, config | 0x0007);
    // 40us delay to recover from powerdown (p14 of spec)
    usleep(40);
}
void INA219::reset() { writeRegister(__REG_CONFIG, __RST); }
auto INA219::voltage() -> float {
    uint16_t value = readRegister(__REG_BUSVOLTAGE) >> 3;
    return static_cast<float>(value * __BUS_MILLIVOLTS_LSB / 1000.0F);
}
auto INA219::shuntVoltage() -> float {
    uint16_t shunt_voltage = readRegister(__REG_SHUNTVOLTAGE);
    return static_cast<float>(__SHUNT_MILLIVOLTS_LSB) * shunt_voltage;
}
auto INA219::supplyVoltage() -> float { return voltage() + (shuntVoltage() / 1000.0F); }
auto INA219::current() -> float {
    uint16_t current_raw = readRegister(__REG_CURRENT);
    auto current = static_cast<int16_t>(current_raw);
    if (current > 32766) {
        current -= static_cast<int16_t>(65536);
    }
    return current * _current_lsb * 1000.0F;
}
auto INA219::power() -> float {
    uint16_t power_raw = readRegister(__REG_POWER);
    auto power = static_cast<int16_t>(power_raw);
    return power * _power_lsb * 1000.0F;
}
