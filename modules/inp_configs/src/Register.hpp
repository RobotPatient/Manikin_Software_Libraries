#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <stdint.h>
#include <Protocol_Constants.hpp>

class INP_Register {
  private:
    RegisterAddress_t address;
    uint8_t value;
    AccessTypes_t accessType;
  public:
    explicit INP_Register(RegisterAddress_t _address);
    explicit INP_Register(RegisterAddress_t _address, AccessTypes_t access);
    void setValue(uint8_t _value);
    uint8_t getValue(void);
    RegisterAddress_t getAddress(void);
    AccessTypes_t getAccesstype(void);
};

#endif // REGISTER_HPP