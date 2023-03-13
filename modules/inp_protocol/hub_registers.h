#ifndef HUB_REGISTERS_H
#define HUB_REGISTERS_H

typedef enum sensor_requests {
    kCompressionReading         = 0x20,
    kDifferentialReading        = 0x21,
    kFingerPositionReading      = 0x22,
} reg_inp_t;

#endif // HUB_REGISTERS_H