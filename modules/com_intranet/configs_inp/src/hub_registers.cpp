#include <hub_registers.hpp>

HubRegisters::HubRegisters(uint8_t reg_amount) {
    registers_counter_ = reg_amount;
    regs = new INP_register*[registers_counter_]; // Dynamic memory allocation !
    for(uint8_t i = 0; i < registers_counter_; i++) {
        regs[i] = new INP_register(static_cast<RegisterAddress_t>(i + 1));
    }
}

HubRegisters::~HubRegisters() {
    delete[] regs; // Delete the array pointers
}

INP_register* HubRegisters::getRegister(RegisterAddress_t address) {
    for (uint8_t i = 0; i < registers_counter_; i++) {
        if (regs[i]->getAddress() == address) {
            return regs[i];
        }
    } 
    return nullptr; // When no register is found.
}

void HubRegisters::write_register(RegisterAddress_t addr, uint8_t val) {
    INP_register* reg = getRegister(addr);
    if(reg != nullptr) {
        reg->setValue(val);
    }
}

uint8_t HubRegisters::read_register(RegisterAddress_t addr) {
    INP_register* reg = getRegister(addr);
    if(reg != nullptr) {
        return (reg->getValue());
    } else {
        return 0;
    }
}

uint8_t HubRegisters::get_register_counter(void) {
    return registers_counter_;
}
