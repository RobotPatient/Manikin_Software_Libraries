#ifndef HUB_I2C_HPP
#define HUB_I2C_HPP

#include <Arduino.h>
#include <Wire.h>
#include <HubRegisters.hpp>

#define SLAVE_ADDR 5 // testing define...

class HubCommunication {
 public:
    explicit HubCommunication() {}

    ~HubCommunication() {}

    void init() {
        Wire.begin(SLAVE_ADDR);                // join i2c bus with address
        Wire.onReceive(receiveEvent);             // register event
        Wire.onRequest(requestEvent);             // register event
    }

 private:
    static HubRegisters hubReg_;

    static void receiveEvent(int rxBytes) {
        if(Wire.available() > 1) {
            uint8_t registerAddress = Wire.read();
            uint8_t value = Wire.read();
            HubCommunication::hubReg_.write_register(static_cast<RegisterAddress_t>(registerAddress), value);
            Serial.print("Received write to: ");
            Serial.println(registerAddress);
            Serial.print("Value: ");
            Serial.println(value);
        }
    }

    static void requestEvent() {
        uint8_t registerAddress = Wire.read();
        uint8_t val = HubCommunication::hubReg_.read_register(static_cast<RegisterAddress_t>(registerAddress));
        Wire.write(val);
        Serial.print("Send Register: ");
        Serial.println(registerAddress);
        Serial.print("Value: ");
        Serial.println(val);
    }
};

HubRegisters HubCommunication::hubReg_(3);

#endif // HUB_I2C_HPP