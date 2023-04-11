#include <HubRegisters.hpp>

HubRegisters::HubRegisters(uint8_t reg_amount) {
    registers_counter_ = reg_amount;
    regs = new INP_Register*[registers_counter_]; // Memory dynamic allocation !
    for(uint8_t i = 0; i < registers_counter_; i++) {
        regs[i] = new INP_Register(static_cast<RegisterAddress_t>(i + 1));
    }
}

HubRegisters::~HubRegisters() {
    delete[] regs; // Delete the array pointers
}

INP_Register* HubRegisters::getRegister(RegisterAddress_t address) {
    for (uint8_t i = 0; i < registers_counter_; i++) {
        if (regs[i]->getAddress() == address) {
            return regs[i];
        }
    } 
    return nullptr; // When no register is found.
}

void HubRegisters::write_register(RegisterAddress_t addr, uint8_t val) {
    INP_Register* reg = getRegister(addr);
    if(reg != nullptr) {
        reg->setValue(val);
    }
}

uint8_t HubRegisters::read_register(RegisterAddress_t addr) {
    INP_Register* reg = getRegister(addr);
    if(reg != nullptr) {
        return (reg->getValue());
    } else {
        return 0;
    }
}
