#ifndef HUB_REGISTERS_HPP
#define HUB_REGISTERS_HPP

#include <stdint.h>
#include <Register.hpp>

class HubRegisters {
  private:
    INP_Register** regs;        // Array of pointers, undefined length.
    uint8_t registers_counter_; // Length of the array

  public:
    explicit HubRegisters(uint8_t reg_amount);
    ~HubRegisters();

    INP_Register* getRegister(RegisterAddress_t address);
    void write_register(RegisterAddress_t addr, uint8_t val);
    uint8_t read_register(RegisterAddress_t addr);
};

#endif // HUB_REGISTERS_HPP