#ifndef HUB_REGISTERS_H
#define HUB_REGISTERS_H

typedef enum {
    kCompression                   = 0x00,
    kDifferential_pressure         = 0x01,
    kFinger_position               = 0x02,
    kGyro                          = 0x03,
} sensor_type_id_t;

typedef enum {
    kSensorhubOne = 0,
    kSensorhubTwo,
} slave_address_t;

typedef enum {
    kStatus = 0,
    kSensorPortA1,
    kSensorPortA2,
    kControlPortA1,
    kControlPortA2,
} slave_reg_addr_t;

#endif // HUB_REGISTERS_H