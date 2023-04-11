#include <Register.hpp>

INP_Register::INP_Register(RegisterAddress_t _address) {
    address = _address;
    value = 0;
    accessType = READ_ONLY;
}

INP_Register::INP_Register(RegisterAddress_t _address, AccessTypes_t access) {
    address = _address;
    value = 0;
    accessType = access;
}

void INP_Register::setValue(uint8_t _value) {
    value = _value;
}

uint8_t INP_Register::getValue(void) {
    return value;
}

RegisterAddress_t INP_Register::getAddress(void) {
    return address;
}

AccessTypes_t INP_Register::getAccesstype(void) {
    return accessType;
}

