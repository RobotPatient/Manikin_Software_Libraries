#include <inp_register.hpp>

INP_register::INP_register(RegisterAddress_t _address) {
    address = _address;
    value = 0;
    accessType = READ_ONLY;
}

INP_register::INP_register(RegisterAddress_t _address, AccessTypes_t access) {
    address = _address;
    value = 0;
    accessType = access;
}

void INP_register::setValue(uint8_t _value) {
    value = _value;
}

uint8_t INP_register::getValue(void) {
    return value;
}

RegisterAddress_t INP_register::getAddress(void) {
    return address;
}

AccessTypes_t INP_register::getAccesstype(void) {
    return accessType;
}

