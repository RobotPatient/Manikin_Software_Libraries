#ifndef INP_REGISTER_HPP
#define INP_REGISTER_HPP

#include <stdint.h>
#include <protocol_references.hpp>

class INP_register {
  private:
    RegisterAddress_t address;
    uint8_t value;
    AccessTypes_t accessType;
  public:
    explicit INP_register(RegisterAddress_t _address);
    explicit INP_register(RegisterAddress_t _address, AccessTypes_t access);
    void setValue(uint8_t _value);
    uint8_t getValue(void);
    RegisterAddress_t getAddress(void);
    AccessTypes_t getAccesstype(void);
};

#endif // INP_REGISTER_HPP