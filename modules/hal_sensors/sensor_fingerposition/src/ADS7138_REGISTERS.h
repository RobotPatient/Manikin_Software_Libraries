#ifndef ADS7138_REGISTERS_H
#define ADS7138_REGISTERS_H

#define ADS7138_ADDR    0x10
#define TWO_BYTE        2

enum CHIP_REGISTERS {
    GENERAL_CFG      =   0x01,
    PIN_CFG          =   0x5,
    SEQUENCE_CFG     =   0x10,
    AUTO_SEQ_CH_SEL  =   0x12,
};

enum CHIP_OPCODES {
    SINGLE_R         = 0b00010000,
    SINGLE_W         = 0b00001000,
    SET_BIT          = 0b00011000,
    CLEAR_BIT        = 0b00100000,
    CONTINUOUS_R     = 0b00110000,
    CONTINUOUS_W     = 0b00101000,
};

enum SENSOR_MAP_INDEX {
    LOWER            = 5,
    MID_L            = 4,
    MID_M            = 3,
    MID_H            = 6,
    RE_L             = 7,
    RE_H             = 0,
    LI_L             = 2,
    LI_H             = 1,       
};

#endif // ADS7138_REGISTERS_H