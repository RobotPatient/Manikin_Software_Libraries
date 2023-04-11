#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <stdint.h>
#include <Register_adresses.hpp>

class INP_Register {
  private:
    RegisterAddress_t address;
    uint8_t value;
  public:
    explicit INP_Register(RegisterAddress_t _address) {
      address = _address;
      value = 0;
    }

    RegisterAddress_t getAddress() {
      return address;
    }

    void setValue(uint8_t _value) {
      value = _value;
    }

    uint8_t getValue() {
      return value;
    }
};

#endif // REGISTER_HPP