#ifndef HUB_REGISTERS_HPP
#define HUB_REGISTERS_HPP

#include <stdint.h>
#include <Register.hpp>

class HubRegisters {
  private:
    INP_Register** regs;        // Array of pointers, undefined length.
    uint8_t registers_counter_; // Length of the array

  public:
    explicit HubRegisters(uint8_t reg_amount) {
        registers_counter_ = reg_amount;
        regs = new INP_Register*[registers_counter_]; // Memory dynamic allocation !
        for(uint8_t i = 0; i < registers_counter_; i++) {
            regs[i] = new INP_Register(static_cast<RegisterAddress_t>(i + 1));
        }
    }

    ~HubRegisters() {
        delete[] regs; // Delete the array pointers
    }

    INP_Register* getRegister(RegisterAddress_t address) {
        for (uint8_t i = 0; i < registers_counter_; i++) {
            if (regs[i]->getAddress() == address) {
                return regs[i];
            }
        } 
        return nullptr; // When no register is found.
    }

    void write_register(RegisterAddress_t addr, uint8_t val) {
        INP_Register* reg = getRegister(addr);
        if(reg != nullptr) {
            reg->setValue(val);
        }
    }

    uint8_t read_register(RegisterAddress_t addr) {
        INP_Register* reg = getRegister(addr);
        if(reg != nullptr) {
            return (reg->getValue());
        } else {
            return 0;
        }
    }
};

#endif // HUB_REGISTERS_HPP