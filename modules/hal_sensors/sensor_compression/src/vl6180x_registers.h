#ifndef SENSOR_COMPRESSION_VL6180X_REGISTERS_H
#define SENSOR_COMPRESSION_VL6180X_REGISTERS_H

const uint8_t kVl6180XFailureReset = -1;
const uint16_t kVl6180XIdentificationModelId = 0x0000;
const uint16_t kVl6180XIdentificationModelRevMajor = 0x0001;
const uint16_t kVl6180XIdentificationModelRevMinor = 0x0002;
const uint16_t kVl6180XIdentificationModuleRevMajor = 0x0003;
const uint16_t kVl6180XIdentificationModuleRevMinor = 0x0004;
const uint16_t kVl6180XIdentificationDate = 0x0006; // 16bit value
const uint16_t kVl6180XIdentificationTime = 0x0008; // 16bit value

const uint16_t kVl6180XSystemModeGpio1 = 0x0011;
const uint16_t kVl6180XSystemInterruptConfigGpio = 0x0014;
const uint16_t kVl6180XSystemInterruptClear = 0x0015;
const uint16_t kVl6180XSystemFreshOutOfReset = 0x0016;

const uint16_t kVl6180XSysrangeStart = 0x0018;
const uint16_t kVl6180XSysrangeIntermeasurementPeriod = 0x001B;
const uint16_t kVl6180XSysrangeMaxConvergenceTime = 0x001C;
const uint16_t kVl6180XSysrangeEarlyConvergenceEstimate = 0x0022;
const uint16_t kVl6180XSysrangeRangeCheckEnables = 0x002D;
const uint16_t kVl6180XSysrangeVhvRecalibrate = 0x002E;
const uint16_t kVl6180XSysrangeVhvRepeatRate = 0x0031;

const uint16_t kVl6180XSysalsStart = 0x0038;
const uint16_t kVl6180XSysalsIntermeasurementPeriod = 0x003E;
const uint16_t kVl6180XSysalsAnalogueGain = 0x003F;
const uint16_t kVl6180XSysalsIntegrationPeriod = 0x0040;

const uint16_t kVl6180XResultAlsVal = 0x0050;
const uint16_t kVl6180XResultRangeVal = 0x0062;

const uint16_t kVl6180XReadoutAveragingSamplePeriod = 0x010A;
const uint16_t kVl6180XFirmwareResultScaler = 0x0120;
const uint16_t kVl6180Xi2CSlaveDeviceAddress = 0x0212;

#endif // SENSOR_COMPRESSION_VL6180X_REGISTERS_H
