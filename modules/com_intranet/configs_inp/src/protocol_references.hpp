#ifndef PROTOCOL_REFERENCES_HPP
#define PROTOCOL_REFERENCES_HPP

typedef enum RegisterAddress {
  REG_ADDRESS_1 = 0x01,
  REG_ADDRESS_2 = 0x02,
  REG_ADDRESS_3 = 0x03,
  STATUS_REG,
} RegisterAddress_t;
  
///@brief Hubs start at 0x30.
typedef enum SlavesAddress {
  HUB_ONE       = 0x30,
  HUB_TWO
} SlavesAddress_t;

typedef enum AccessTypes {
  READ_ONLY,
  WRITE_ONLY,
  READ_WRITE
} AccessTypes_t;

#endif // PROTOCOL_REFERENCES_HPP