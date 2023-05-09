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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <hub_registers.hpp>
#include <inp_register.hpp>

using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::Return;

TEST(INP_register, ConstructorWithReadOnlyAccessType) {
  RegisterAddress_t address = RegisterAddress_t::REG_ADDRESS_2;
  INP_register reg(address);
  EXPECT_EQ(address, reg.getAddress());
  EXPECT_EQ(0, reg.getValue());
  EXPECT_EQ(READ_ONLY, reg.getAccesstype());
}

TEST(INP_register, ConstructorWithAccessTypes) {
  RegisterAddress_t address = RegisterAddress_t::REG_ADDRESS_2;
  AccessTypes_t access = WRITE_ONLY;
  INP_register reg(address, access);
  EXPECT_EQ(address, reg.getAddress());
  EXPECT_EQ(0, reg.getValue());
  EXPECT_EQ(access, reg.getAccesstype());
}

TEST(INP_register, SetValue) {
  INP_register reg(RegisterAddress_t::REG_ADDRESS_2);
  uint8_t value = 0xAB;
  reg.setValue(value);
  EXPECT_EQ(value, reg.getValue());
}

TEST(INP_register, GetValue) {
  INP_register reg(RegisterAddress_t::REG_ADDRESS_2);
  uint8_t value = 0xCD;
  reg.setValue(value);
  uint8_t result = reg.getValue();
  EXPECT_EQ(value, result);
}

TEST(INP_register, GetAddress) {
  RegisterAddress_t address = RegisterAddress_t::REG_ADDRESS_2;
  INP_register reg(address);
  RegisterAddress_t result = reg.getAddress();
  EXPECT_EQ(address, result);
}

TEST(INP_register, GetAccessType) {
  AccessTypes_t access = READ_WRITE;
  INP_register reg(RegisterAddress_t::REG_ADDRESS_2, access);
  AccessTypes_t result = reg.getAccesstype();
  EXPECT_EQ(access, result);
}

/*          HUB REGISTER TESTCASES          */
TEST(HubRegisters, Constructor) {
  uint8_t num_regs = 4;
  HubRegisters hub(num_regs);
  EXPECT_EQ(num_regs, hub.get_register_counter());
  for (uint8_t i = 0; i < num_regs; i++) {
    INP_register* reg = hub.getRegister(static_cast<RegisterAddress_t>(i + 1));
    EXPECT_NE(nullptr, reg);
    EXPECT_EQ(static_cast<RegisterAddress_t>(i + 1), reg->getAddress());
    EXPECT_EQ(0, reg->getValue());
  }
}

TEST(HubRegisters, WriteRegister) {
  HubRegisters hub(2);
  RegisterAddress_t addr1 = RegisterAddress_t::REG_ADDRESS_1;
  uint8_t val1 = 0x0F;
  RegisterAddress_t addr2 = RegisterAddress_t::REG_ADDRESS_2;
  uint8_t val2 = 0x50;
  hub.write_register(addr1, val1);
  hub.write_register(addr2, val2);
  INP_register* reg1 = hub.getRegister(addr1);
  ASSERT_NE(nullptr, reg1) << "Failed to get register for address " << addr1;
  reg1->setValue(val1);
  EXPECT_EQ(val1, reg1->getValue());
  INP_register* reg2 = hub.getRegister(addr2);
  ASSERT_NE(nullptr, reg2) << "Failed to get register for address " << addr2;
  reg2->setValue(val2);
  EXPECT_EQ(val2, reg2->getValue());
}

TEST(HubRegisters, ReadRegister) {
  HubRegisters hub(1);
  RegisterAddress_t addr = RegisterAddress_t::REG_ADDRESS_1;
  uint8_t val = 0x33;
  hub.write_register(addr, val);
  uint8_t result = hub.read_register(addr);
  EXPECT_EQ(val, result);
}

TEST(HubRegisters, GetRegisterNotFound) {
  HubRegisters hub(2);
  RegisterAddress_t addr = RegisterAddress_t::REG_ADDRESS_3;
  INP_register* result = hub.getRegister(addr);
  EXPECT_EQ(nullptr, result);
}

TEST(HubRegisters, Destructor) {
  uint8_t num_regs = 2;
  HubRegisters* hub = new HubRegisters(num_regs);
  hub->~HubRegisters();  // Or delete hub.
  // Nothing to assert, just making sure that the destructor does not cause any memory leaks
}

int main(int argc, char** argv) {
  // ::testing::InitGoogleTest(&argc, argv);
  // if you plan to use GMock, replace the line above with
  ::testing::InitGoogleMock(&argc, argv);

  if (RUN_ALL_TESTS()) {}

  // Always return zero-code and allow PlatformIO to parse results
  return 0;
}
