#include "ADS7138.hpp"

#ifdef __arm__
#include "Arduino.h"
#define sleep(ms) delay(ms)
#else
#define sleep(ms) usleep(1000*ms)
#endif


uint16_t createReg(uint8_t opcode, uint8_t regaddr){
return (opcode | ((regaddr << 8)));
}


void ADS7138::writeRegister(uint8_t regAddr, uint8_t data) {
    uint16_t reg = createReg(CONTINUOUS_W, regAddr);
    _i2c_handle->write_reg(reg, data);              // Send address and register address bytes
}

void ADS7138::setRegister(uint8_t regAddr, uint8_t data) {
    uint16_t reg = createReg(SET_BIT, regAddr);
    _i2c_handle->write_reg(reg, data);                      // Send address and register address bytes
}

void ADS7138::clearRegister(uint8_t regAddr, uint8_t data) {
    uint16_t reg = createReg(CLEAR_BIT, regAddr);
    _i2c_handle->write_reg(reg, data);                  // Send address and register address bytes
}

uint8_t ADS7138::getRegister(uint8_t registerAddr) {
    uint16_t reg = createReg(SINGLE_R, registerAddr);                      // Read Data from selected register
    return _i2c_handle->read_reg(reg);
}

void ADS7138::initDefaultRead(void) {
    setRegister(PIN_CFG, 0x0);                  // Channels are configured as Analog inps
    setRegister(GENERAL_CFG, 0b10);             // SET CAL bit
    setRegister(AUTO_SEQ_CH_SEL, 0xFF);         // xF --> Set all adc channels as inputs. enabled in scanning sequence.
    setRegister(SEQUENCE_CFG, 0b01);            // Set Auto sequence mode on = 1. And 4th for sequence start.
}
void ADS7138::startReadSEQ(void) {
    setRegister(SEQUENCE_CFG, 1 << 4);          // 4th bit starts the sequence.
}

void ADS7138::stopReadSEQ(void) {
    clearRegister(SEQUENCE_CFG, 1 << 4);        // 4th bit reset the sequence.
}

void ADS7138::readADC(uint16_t *dest) {
    startReadSEQ();
    uint8_t temp[2] = {0x0, 0x0};
    uint16_t buf[8];

    for(int i = 0; i < 8; i++) {
        getReading(temp);
        buf[i] = (temp[0] << 4) | (temp[1] >> 4); // 12b conversion.
        sleep(50);
    }
    reindexArray(dest, buf);
    stopReadSEQ();
}

void ADS7138::reindexArray(uint16_t *dest, uint16_t *original) {
    dest[0] = original[LOWER];
    dest[1] = original[MID_L];
    dest[2] = original[MID_M];
    dest[3] = original[MID_H];
    dest[4] = original[RE_L];
    dest[5] = original[RE_H];
    dest[6] = original[LI_L];
    dest[7] = original[LI_H];
}

void ADS7138::getReading(uint8_t *buf) {
    _i2c_handle->read_bytes(buf, TWO_BYTE);
}
