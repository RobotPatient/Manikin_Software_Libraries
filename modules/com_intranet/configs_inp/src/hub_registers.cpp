/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Richard Kroesen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 ***********************************************************************************************/

#include <hub_registers.hpp>

HubRegisters::HubRegisters(uint8_t reg_amount) {
  registers_counter_ = reg_amount;
  regs = new INP_register*[registers_counter_];  // Dynamic memory allocation !
  for (uint8_t i = 0; i < registers_counter_; i++) {
    regs[i] = new INP_register(static_cast<RegisterAddress_t>(i + 1));
  }
}

HubRegisters::~HubRegisters() {
  delete[] regs;  // Delete the array pointers
}

INP_register* HubRegisters::getRegister(RegisterAddress_t address) {
  for (uint8_t i = 0; i < registers_counter_; i++) {
    if (regs[i]->getAddress() == address) {
      return regs[i];
    }
  }
  return nullptr;  // When no register is found.
}

void HubRegisters::write_register(RegisterAddress_t addr, uint8_t val) {
  INP_register* reg = getRegister(addr);
  if (reg != nullptr) {
    reg->setValue(val);
  }
}

uint8_t HubRegisters::read_register(RegisterAddress_t addr) {
  INP_register* reg = getRegister(addr);
  if (reg != nullptr) {
    return (reg->getValue());
  } else {
    return 0;
  }
}

uint8_t HubRegisters::get_register_counter(void) {
  return registers_counter_;
}
